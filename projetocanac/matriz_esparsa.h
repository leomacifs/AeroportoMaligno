#ifndef MATRIZ_ESPARSA_H
#define MATRIZ_ESPARSA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================
 *  Nó da lista encadeada (elemento da matriz esparsa)
 * ============================================================ */
typedef struct No {
    int coluna;          /* índice da coluna (aeroporto destino) */
    void *dado;          /* ponteiro genérico para o dado armazenado */
    struct No *prox;     /* próximo nó na mesma linha */
} No;

/* ============================================================
 *  Linha da matriz esparsa (lista de adjacência de um vértice)
 * ============================================================ */
typedef struct {
    No *inicio;          /* primeiro nó da linha */
} LinhaMatriz;

/* ============================================================
 *  Matriz esparsa completa
 * ============================================================ */
typedef struct {
    LinhaMatriz *linhas; /* vetor de linhas (uma por vértice) */
    int capacidade;      /* número de linhas alocadas */
    int tamanho_dado;    /* tamanho em bytes do tipo de dado armazenado */
} MatrizEsparsa;

/* ---- Protótipos ---- */
MatrizEsparsa *matriz_criar(int capacidade_inicial, int tamanho_dado);
void          matriz_destruir(MatrizEsparsa *m);
void          matriz_expandir(MatrizEsparsa *m, int nova_capacidade);
int           matriz_inserir(MatrizEsparsa *m, int linha, int coluna, void *dado);
int           matriz_remover(MatrizEsparsa *m, int linha, int coluna,
                             int (*comparar)(void *dado, void *chave), void *chave);
No           *matriz_buscar(MatrizEsparsa *m, int linha, int coluna);
void          matriz_listar_linha(MatrizEsparsa *m, int linha,
                                  void (*imprimir)(void *dado, int coluna));
void          matriz_listar_tudo(MatrizEsparsa *m, int num_vertices,
                                 void (*imprimir)(void *dado, int linha, int coluna));

#endif /* MATRIZ_ESPARSA_H */
