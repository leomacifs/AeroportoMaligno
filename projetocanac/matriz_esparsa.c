#include "matriz_esparsa.h"

/* ============================================================
 *  matriz_criar
 *  Aloca e inicializa a matriz esparsa com capacidade_inicial linhas.
 * ============================================================ */
MatrizEsparsa *matriz_criar(int capacidade_inicial, int tamanho_dado) {
    MatrizEsparsa *m = (MatrizEsparsa *)malloc(sizeof(MatrizEsparsa));
    if (!m) return NULL;

    m->linhas = (LinhaMatriz *)calloc(capacidade_inicial, sizeof(LinhaMatriz));
    if (!m->linhas) { free(m); return NULL; }

    m->capacidade   = capacidade_inicial;
    m->tamanho_dado = tamanho_dado;
    return m;
}

/* ============================================================
 *  matriz_destruir
 *  Libera todos os nós e a estrutura principal.
 * ============================================================ */
void matriz_destruir(MatrizEsparsa *m) {
    if (!m) return;
    for (int i = 0; i < m->capacidade; i++) {
        No *atual = m->linhas[i].inicio;
        while (atual) {
            No *tmp = atual->prox;
            free(atual->dado);
            free(atual);
            atual = tmp;
        }
    }
    free(m->linhas);
    free(m);
}

/* ============================================================
 *  matriz_expandir
 *  Aumenta o vetor de linhas para nova_capacidade.
 * ============================================================ */
void matriz_expandir(MatrizEsparsa *m, int nova_capacidade) {
    if (!m || nova_capacidade <= m->capacidade) return;

    LinhaMatriz *novas = (LinhaMatriz *)realloc(m->linhas,
                          nova_capacidade * sizeof(LinhaMatriz));
    if (!novas) return;

    /* Inicializa as novas linhas com NULL */
    for (int i = m->capacidade; i < nova_capacidade; i++)
        novas[i].inicio = NULL;

    m->linhas     = novas;
    m->capacidade = nova_capacidade;
}

/* ============================================================
 *  matriz_inserir
 *  Insere (linha, coluna) com uma cópia de *dado.
 *  Retorna 1 em sucesso, 0 em falha.
 * ============================================================ */
int matriz_inserir(MatrizEsparsa *m, int linha, int coluna, void *dado) {
    if (!m || linha < 0 || linha >= m->capacidade ||
        coluna < 0 || coluna >= m->capacidade) return 0;

    No *novo = (No *)malloc(sizeof(No));
    if (!novo) return 0;

    novo->dado = malloc(m->tamanho_dado);
    if (!novo->dado) { free(novo); return 0; }

    memcpy(novo->dado, dado, m->tamanho_dado);
    novo->coluna = coluna;
    novo->prox   = m->linhas[linha].inicio;
    m->linhas[linha].inicio = novo;
    return 1;
}

/* ============================================================
 *  matriz_remover
 *  Remove o primeiro nó da linha cujo dado satisfaz comparar(dado,chave)==0.
 *  Retorna 1 se removeu, 0 se não encontrou.
 * ============================================================ */
int matriz_remover(MatrizEsparsa *m, int linha, int coluna,
                   int (*comparar)(void *dado, void *chave), void *chave) {
    if (!m || linha < 0 || linha >= m->capacidade) return 0;

    No **anterior = &m->linhas[linha].inicio;
    No  *atual    = m->linhas[linha].inicio;

    while (atual) {
        /* Filtra por coluna (se coluna >= 0) e por chave (se comparar fornecido) */
        int col_ok  = (coluna < 0) || (atual->coluna == coluna);
        int dado_ok = (!comparar) || (comparar(atual->dado, chave) == 0);

        if (col_ok && dado_ok) {
            *anterior = atual->prox;
            free(atual->dado);
            free(atual);
            return 1;
        }
        anterior = &atual->prox;
        atual    = atual->prox;
    }
    return 0;
}

/* ============================================================
 *  matriz_buscar
 *  Retorna o nó na posição (linha, coluna) ou NULL.
 * ============================================================ */
No *matriz_buscar(MatrizEsparsa *m, int linha, int coluna) {
    if (!m || linha < 0 || linha >= m->capacidade) return NULL;

    No *atual = m->linhas[linha].inicio;
    while (atual) {
        if (atual->coluna == coluna) return atual;
        atual = atual->prox;
    }
    return NULL;
}

/* ============================================================
 *  matriz_listar_linha
 *  Percorre todos os nós de uma linha chamando imprimir(dado, coluna).
 * ============================================================ */
void matriz_listar_linha(MatrizEsparsa *m, int linha,
                         void (*imprimir)(void *dado, int coluna)) {
    if (!m || linha < 0 || linha >= m->capacidade || !imprimir) return;

    No *atual = m->linhas[linha].inicio;
    while (atual) {
        imprimir(atual->dado, atual->coluna);
        atual = atual->prox;
    }
}

/* ============================================================
 *  matriz_listar_tudo
 *  Percorre todas as linhas/colunas chamando imprimir(dado, linha, coluna).
 * ============================================================ */
void matriz_listar_tudo(MatrizEsparsa *m, int num_vertices,
                        void (*imprimir)(void *dado, int linha, int coluna)) {
    if (!m || !imprimir) return;
    int lim = (num_vertices < m->capacidade) ? num_vertices : m->capacidade;
    for (int i = 0; i < lim; i++) {
        No *atual = m->linhas[i].inicio;
        while (atual) {
            imprimir(atual->dado, i, atual->coluna);
            atual = atual->prox;
        }
    }
}
