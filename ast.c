#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

AST* novo_no(const char *tipo, const char *valor) {
    AST *n = malloc(sizeof(AST));
    strcpy(n->tipo, tipo);
    if (valor) strcpy(n->valor, valor);
    n->n_filhos = 0;
    return n;
}

void print_ast(AST *n, int nivel) {
    for (int i = 0; i < nivel; i++) printf("  ");
    printf("%s (%s)\n", n->tipo, n->valor);

    for (int i = 0; i < n->n_filhos; i++) {
        print_ast(n->filhos[i], nivel + 1);
    }
}