#ifndef LEXICAL_H
#define LEXICAL_H

#include <stdio.h>

#define MAX_TOKEN 256

typedef struct {
    char classe[32];
    char lexema[64];
    int linha;
    int coluna;
} Token;





extern Token tokens[1000];
extern int total_tokens;


void analisar_arquivo(FILE *in, FILE *out);

#endif