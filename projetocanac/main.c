#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo_aeroportos.h"

/* ============================================================
 *  Leitura segura de strings
 * ============================================================ */
static void ler_string(const char *prompt, char *buf, int max) {
    printf("%s", prompt);
    if (fgets(buf, max, stdin)) {
        /* Se a linha não coube inteira, descarta o restante */
        if (buf[strcspn(buf, "\n")] != '\n') {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
        buf[strcspn(buf, "\n")] = '\0';
    }
}

static int ler_inteiro(const char *prompt) {
    char buf[32];
    ler_string(prompt, buf, sizeof(buf));
    return atoi(buf);
}

/* ============================================================
 *  Menu principal
 * ============================================================ */
static void exibir_menu(void) {
    printf("\n========================================\n");
    printf("   ANAC - Sistema de Malha Aerea\n");
    printf("========================================\n");
    printf("  1. Cadastrar aeroporto\n");
    printf("  2. Cadastrar voo\n");
    printf("  3. Remover voo\n");
    printf("  4. Listar voos de um aeroporto\n");
    printf("  5. Listar trajetos entre aeroportos\n");
    printf("  6. Listar todos os aeroportos\n");
    printf("  0. Sair\n");
    printf("----------------------------------------\n");
    printf("Opcao: ");
}

/* ============================================================
 *  Handlers de cada opção
 * ============================================================ */
static void op_cadastrar_aeroporto(GrafoAeroportos *g) {
    char codigo[COD_MAX];
    char cidade[CIDADE_MAX];
    ler_string("Codigo IATA (ex: GRU): ", codigo, sizeof(codigo));
    ler_string("Nome da cidade       : ", cidade,  sizeof(cidade));

    /* Converte código para maiúsculas */
    for (int i = 0; codigo[i]; i++)
        if (codigo[i] >= 'a' && codigo[i] <= 'z') codigo[i] -= 32;

    grafo_cadastrar_aeroporto(g, codigo, cidade);
}

static void op_cadastrar_voo(GrafoAeroportos *g) {
    char orig[COD_MAX], dest[COD_MAX];
    int  num;

    ler_string("Codigo aeroporto ORIGEM : ", orig, sizeof(orig));
    ler_string("Codigo aeroporto DESTINO: ", dest, sizeof(dest));
    num = ler_inteiro("Numero do voo           : ");

    for (int i = 0; orig[i]; i++)
        if (orig[i] >= 'a' && orig[i] <= 'z') orig[i] -= 32;
    for (int i = 0; dest[i]; i++)
        if (dest[i] >= 'a' && dest[i] <= 'z') dest[i] -= 32;

    grafo_cadastrar_voo(g, num, orig, dest);
}

static void op_remover_voo(GrafoAeroportos *g) {
    int num = ler_inteiro("Numero do voo a remover: ");
    grafo_remover_voo(g, num);
}

static void op_listar_voos_de(GrafoAeroportos *g) {
    char cod[COD_MAX];
    ler_string("Codigo do aeroporto de origem: ", cod, sizeof(cod));
    for (int i = 0; cod[i]; i++)
        if (cod[i] >= 'a' && cod[i] <= 'z') cod[i] -= 32;
    grafo_listar_voos_de(g, cod);
}

static void op_listar_trajetos(GrafoAeroportos *g) {
    char orig[COD_MAX], dest[COD_MAX];
    ler_string("Codigo aeroporto ORIGEM : ", orig, sizeof(orig));
    ler_string("Codigo aeroporto DESTINO: ", dest, sizeof(dest));
    for (int i = 0; orig[i]; i++)
        if (orig[i] >= 'a' && orig[i] <= 'z') orig[i] -= 32;
    for (int i = 0; dest[i]; i++)
        if (dest[i] >= 'a' && dest[i] <= 'z') dest[i] -= 32;
    grafo_listar_trajetos(g, orig, dest);
}

/* ============================================================
 *  Dados iniciais de teste (conforme o enunciado)
 * ============================================================ */
static void carregar_dados_teste(GrafoAeroportos *g) {
    printf("\n--- Carregando dados de teste do enunciado ---\n");

    /* Aeroportos */
    grafo_cadastrar_aeroporto(g, "BSB", "Brasilia");
    grafo_cadastrar_aeroporto(g, "CNF", "Belo Horizonte");
    grafo_cadastrar_aeroporto(g, "GIG", "Rio de Janeiro");
    grafo_cadastrar_aeroporto(g, "GRU", "Sao Paulo");
    grafo_cadastrar_aeroporto(g, "SSA", "Salvador");

    /* Voos conforme a matriz da Figura 1 */
    /* Linha 0 (BSB): voo 107 para SSA */
    grafo_cadastrar_voo(g, 107, "BSB", "SSA");

    /* Linha 1 (CNF): voo 555 -> GIG, 101 -> GRU, 214 -> SSA */
    grafo_cadastrar_voo(g, 555, "CNF", "GIG");
    grafo_cadastrar_voo(g, 101, "CNF", "GRU");
    grafo_cadastrar_voo(g, 214, "CNF", "SSA");

    /* Linha 2 (GIG): voo 554 -> CNF, 090 -> GRU */
    grafo_cadastrar_voo(g, 554, "GIG", "CNF");
    grafo_cadastrar_voo(g, 90,  "GIG", "GRU");

    /* Linha 3 (GRU): voo 050 -> BSB, 102 -> CNF, 089 -> GIG */
    grafo_cadastrar_voo(g, 50,  "GRU", "BSB");
    grafo_cadastrar_voo(g, 102, "GRU", "CNF");
    grafo_cadastrar_voo(g, 89,  "GRU", "GIG");

    /* Linha 4 (SSA): voo 215 -> CNF */
    grafo_cadastrar_voo(g, 215, "SSA", "CNF");

    printf("--- Dados de teste carregados com sucesso ---\n");
}

/* ============================================================
 *  main
 * ============================================================ */
int main(void) {
    GrafoAeroportos *g = grafo_criar();
    if (!g) {
        fprintf(stderr, "ERRO FATAL: nao foi possivel criar o grafo.\n");
        return EXIT_FAILURE;
    }

    carregar_dados_teste(g);

    int opcao;
    do {
        exibir_menu();
        opcao = ler_inteiro("");

        switch (opcao) {
            case 1: op_cadastrar_aeroporto(g); break;
            case 2: op_cadastrar_voo(g);       break;
            case 3: op_remover_voo(g);         break;
            case 4: op_listar_voos_de(g);      break;
            case 5: op_listar_trajetos(g);     break;
            case 6: grafo_listar_aeroportos(g); break;
            case 0: printf("Saindo...\n");     break;
            default: printf("[AVISO] Opcao invalida.\n");
        }
    } while (opcao != 0);

    grafo_destruir(g);
    return EXIT_SUCCESS;
}
