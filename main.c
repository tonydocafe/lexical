#include <stdio.h>
#include "lexical.h"

int main(int argc, char *argv[]) {

    if (argc < 3) {
        printf("Uso: %s <arquivo_entrada> <arquivo_saida>\n", argv[0]);
        return 1;
    }

    FILE *entrada = fopen(argv[1], "r");
    if (!entrada) {
        printf("Erro ao abrir %s\n", argv[1]);
        return 1;
    }

    FILE *saida = fopen(argv[2], "w");
    if (!saida) {
        printf("Erro ao criar %s\n", argv[2]);
        fclose(entrada);
        return 1;
    }

    analisar_arquivo(entrada, saida);

    fclose(entrada);
    fclose(saida);

    printf("Analise lexica concluida.\n");

    return 0;
}