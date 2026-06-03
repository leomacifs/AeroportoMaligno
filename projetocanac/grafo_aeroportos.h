#ifndef GRAFO_AEROPORTOS_H
#define GRAFO_AEROPORTOS_H

#include "matriz_esparsa.h"

/* ============================================================
 *  Constantes
 * ============================================================ */
#define CAPACIDADE_INICIAL 8
#define COD_MAX            4    /* 3 letras + '\0' */
#define CIDADE_MAX        64

/* ============================================================
 *  Estrutura do aeroporto (vértice)
 * ============================================================ */
typedef struct {
    char codigo[COD_MAX];    /* ex.: "GRU" */
    char cidade[CIDADE_MAX]; /* ex.: "São Paulo" */
} Aeroporto;

/* ============================================================
 *  Estrutura do voo (aresta)
 * ============================================================ */
typedef struct {
    int numero; /* número do voo, ex.: 555 */
} Voo;

/* ============================================================
 *  Grafo principal
 * ============================================================ */
typedef struct {
    Aeroporto    *vertices;    /* vetor dinâmico de aeroportos */
    int           num_vertices;
    int           cap_vertices;
    MatrizEsparsa *matriz;     /* matriz esparsa de adjacência */
} GrafoAeroportos;

/* ---- Protótipos ---- */
GrafoAeroportos *grafo_criar(void);
void             grafo_destruir(GrafoAeroportos *g);

int  grafo_cadastrar_aeroporto(GrafoAeroportos *g,
                                const char *codigo, const char *cidade);
int  grafo_buscar_aeroporto(GrafoAeroportos *g, const char *codigo);

int  grafo_cadastrar_voo(GrafoAeroportos *g, int numero,
                          const char *cod_origem, const char *cod_destino);
int  grafo_remover_voo(GrafoAeroportos *g, int numero_voo);

void grafo_listar_voos_de(GrafoAeroportos *g, const char *cod_origem);
void grafo_listar_trajetos(GrafoAeroportos *g,
                            const char *cod_origem, const char *cod_destino);

void grafo_listar_aeroportos(GrafoAeroportos *g);

#endif /* GRAFO_AEROPORTOS_H */
