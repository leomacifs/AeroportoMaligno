#include "grafo_aeroportos.h"

/* ============================================================
 *  Auxiliar: garante que o vetor de vértices e a matriz
 *  têm capacidade suficiente para pelo menos 'min' entradas.
 * ============================================================ */
static void garantir_capacidade(GrafoAeroportos *g, int min) {
    if (min <= g->cap_vertices) return;

    int nova = g->cap_vertices * 2;
    if (nova < min) nova = min;

    g->vertices = (Aeroporto *)realloc(g->vertices, nova * sizeof(Aeroporto));
    if (!g->vertices) {
        fprintf(stderr, "ERRO: falha ao realocar vetor de aeroportos.\n");
        exit(EXIT_FAILURE);
    }
    matriz_expandir(g->matriz, nova);
    g->cap_vertices = nova;
}

/* ============================================================
 *  grafo_criar
 * ============================================================ */
GrafoAeroportos *grafo_criar(void) {
    GrafoAeroportos *g = (GrafoAeroportos *)malloc(sizeof(GrafoAeroportos));
    if (!g) return NULL;

    g->cap_vertices = CAPACIDADE_INICIAL;
    g->num_vertices = 0;
    g->vertices     = (Aeroporto *)calloc(CAPACIDADE_INICIAL, sizeof(Aeroporto));
    g->matriz       = matriz_criar(CAPACIDADE_INICIAL, sizeof(Voo));

    if (!g->vertices || !g->matriz) {
        free(g->vertices);
        if (g->matriz) matriz_destruir(g->matriz);
        free(g);
        return NULL;
    }
    return g;
}

/* ============================================================
 *  grafo_destruir
 * ============================================================ */
void grafo_destruir(GrafoAeroportos *g) {
    if (!g) return;
    free(g->vertices);
    matriz_destruir(g->matriz);
    free(g);
}

/* ============================================================
 *  grafo_buscar_aeroporto
 *  Retorna o índice do aeroporto com 'codigo', ou -1 se não existe.
 * ============================================================ */
int grafo_buscar_aeroporto(GrafoAeroportos *g, const char *codigo) {
    for (int i = 0; i < g->num_vertices; i++)
        if (strcmp(g->vertices[i].codigo, codigo) == 0) return i;
    return -1;
}

/* ============================================================
 *  grafo_cadastrar_aeroporto
 *  Retorna índice do novo aeroporto, ou -1 em caso de erro.
 * ============================================================ */
int grafo_cadastrar_aeroporto(GrafoAeroportos *g,
                               const char *codigo, const char *cidade) {
    if (!g || !codigo || !cidade) return -1;

    if (grafo_buscar_aeroporto(g, codigo) >= 0) {
        printf("[FALHA] Aeroporto '%s' ja cadastrado.\n", codigo);
        return -1;
    }

    garantir_capacidade(g, g->num_vertices + 1);

    int idx = g->num_vertices++;
    strncpy(g->vertices[idx].codigo, codigo, COD_MAX - 1);
    g->vertices[idx].codigo[COD_MAX - 1] = '\0';
    strncpy(g->vertices[idx].cidade, cidade, CIDADE_MAX - 1);
    g->vertices[idx].cidade[CIDADE_MAX - 1] = '\0';

    printf("[OK] Aeroporto %s (%s) cadastrado no indice %d.\n",
           codigo, cidade, idx);
    return idx;
}

/* ============================================================
 *  grafo_cadastrar_voo
 *  Retorna 1 em sucesso, 0 em falha.
 * ============================================================ */
int grafo_cadastrar_voo(GrafoAeroportos *g, int numero,
                         const char *cod_origem, const char *cod_destino) {
    if (!g) return 0;

    int orig = grafo_buscar_aeroporto(g, cod_origem);
    int dest = grafo_buscar_aeroporto(g, cod_destino);

    if (orig < 0) {
        printf("[FALHA] Aeroporto de origem '%s' nao encontrado.\n", cod_origem);
        return 0;
    }
    if (dest < 0) {
        printf("[FALHA] Aeroporto de destino '%s' nao encontrado.\n", cod_destino);
        return 0;
    }
    if (orig == dest) {
        printf("[FALHA] Origem e destino sao o mesmo aeroporto.\n");
        return 0;
    }
    if (numero <= 0) {
        printf("[FALHA] Numero de voo invalido: %d.\n", numero);
        return 0;
    }

    /* Verifica se o número de voo já existe na rota */
    No *existente = matriz_buscar(g->matriz, orig, dest);
    if (existente && ((Voo *)existente->dado)->numero == numero) {
        printf("[FALHA] Voo %d de %s para %s ja cadastrado.\n",
               numero, cod_origem, cod_destino);
        return 0;
    }

    Voo v = { numero };
    if (!matriz_inserir(g->matriz, orig, dest, &v)) {
        printf("[FALHA] Erro interno ao inserir voo na matriz.\n");
        return 0;
    }

    printf("[OK] Voo %d cadastrado: %s (%s) -> %s (%s).\n",
           numero,
           g->vertices[orig].codigo, g->vertices[orig].cidade,
           g->vertices[dest].codigo, g->vertices[dest].cidade);
    return 1;
}

/* ============================================================
 *  Auxiliar de comparação para remover_voo
 * ============================================================ */
static int comparar_numero_voo(void *dado, void *chave) {
    return ((Voo *)dado)->numero - *((int *)chave);
}

/* ============================================================
 *  grafo_remover_voo
 *  Procura o número de voo em TODAS as linhas e remove a primeira
 *  ocorrência encontrada.  Retorna 1 se removeu, 0 se não achou.
 * ============================================================ */
int grafo_remover_voo(GrafoAeroportos *g, int numero_voo) {
    if (!g) return 0;

    for (int i = 0; i < g->num_vertices; i++) {
        if (matriz_remover(g->matriz, i, -1,
                           comparar_numero_voo, &numero_voo)) {
            printf("[OK] Voo %d removido.\n", numero_voo);
            return 1;
        }
    }
    printf("[FALHA] Voo %d nao encontrado.\n", numero_voo);
    return 0;
}

/* ============================================================
 *  Callback usado em grafo_listar_voos_de
 * ============================================================ */
/* Precisamos do ponteiro para o grafo dentro do callback; usamos
   uma variável estática local temporária (thread-unsafe, mas ok
   para uso didático single-thread). */
static GrafoAeroportos *_g_temp = NULL;

static void imprimir_voo_destino(void *dado, int coluna) {
    Voo *v = (Voo *)dado;
    if (_g_temp && coluna >= 0 && coluna < _g_temp->num_vertices)
        printf("   Voo %d  ->  %s (%s)\n",
               v->numero,
               _g_temp->vertices[coluna].codigo,
               _g_temp->vertices[coluna].cidade);
}

/* ============================================================
 *  grafo_listar_voos_de
 * ============================================================ */
void grafo_listar_voos_de(GrafoAeroportos *g, const char *cod_origem) {
    int orig = grafo_buscar_aeroporto(g, cod_origem);
    if (orig < 0) {
        printf("[FALHA] Aeroporto '%s' nao encontrado.\n", cod_origem);
        return;
    }

    printf("\nVoos a partir de %s (%s):\n",
           g->vertices[orig].codigo, g->vertices[orig].cidade);

    _g_temp = g;
    No *atual = g->matriz->linhas[orig].inicio;
    if (!atual) {
        printf("   (nenhum voo cadastrado)\n");
        return;
    }
    matriz_listar_linha(g->matriz, orig, imprimir_voo_destino);
}

/* ============================================================
 *  grafo_listar_trajetos  –  DFS com registro de caminho
 * ============================================================ */
static int  _visitado[256];
static int  _caminho[256];
static int  _prof;
static int  _destino_idx;
static GrafoAeroportos *_g_dfs;

static void imprimir_caminho(void) {
    printf("   ");
    for (int i = 0; i <= _prof; i++) {
        printf("%s", _g_dfs->vertices[_caminho[i]].codigo);
        if (i < _prof) printf(" -> ");
    }
    printf("\n");
}

static void dfs(int atual) {
    _visitado[atual] = 1;
    _caminho[_prof]  = atual;

    if (atual == _destino_idx) {
        imprimir_caminho();
        _visitado[atual] = 0;
        return;
    }

    _prof++;
    No *no = _g_dfs->matriz->linhas[atual].inicio;
    while (no) {
        if (!_visitado[no->coluna])
            dfs(no->coluna);
        no = no->prox;
    }
    _prof--;
    _visitado[atual] = 0;
}

void grafo_listar_trajetos(GrafoAeroportos *g,
                            const char *cod_origem, const char *cod_destino) {
    int orig = grafo_buscar_aeroporto(g, cod_origem);
    int dest = grafo_buscar_aeroporto(g, cod_destino);

    if (orig < 0) { printf("[FALHA] Origem '%s' nao encontrada.\n",  cod_origem);  return; }
    if (dest < 0) { printf("[FALHA] Destino '%s' nao encontrado.\n", cod_destino); return; }
    if (orig == dest) { printf("[FALHA] Origem e destino identicos.\n"); return; }

    printf("\nTrajetos de %s para %s:\n",
           g->vertices[orig].codigo, g->vertices[dest].codigo);

    memset(_visitado, 0, sizeof(_visitado));
    _prof        = 0;
    _destino_idx = dest;
    _g_dfs       = g;

    dfs(orig);
    printf("(fim da listagem)\n");
}

/* ============================================================
 *  grafo_listar_aeroportos
 * ============================================================ */
void grafo_listar_aeroportos(GrafoAeroportos *g) {
    if (g->num_vertices == 0) {
        printf("Nenhum aeroporto cadastrado.\n");
        return;
    }
    printf("\n%-5s  %-4s  %s\n", "Idx", "Cod", "Cidade");
    printf("-----  ----  --------------------\n");
    for (int i = 0; i < g->num_vertices; i++)
        printf("%-5d  %-4s  %s\n", i,
               g->vertices[i].codigo, g->vertices[i].cidade);
}
