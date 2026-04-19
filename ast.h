
#ifndef AST_H
#define AST_H

typedef struct AST {
    char tipo[32];
    char valor[64];
    struct AST *filhos[10];
    int n_filhos;
} AST;

AST* novo_no(const char *tipo, const char *valor);
void print_ast(AST *n, int nivel);

#endif

