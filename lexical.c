#include <stdio.h>
#include <ctype.h>
#include <regex.h>
#include <string.h>
#include "lexical.h"

#define MAX_TOKEN 256



int match(const char *pattern, char c) {
    regex_t regex;
    char str[2] = {c, '\0'};

    if (regcomp(&regex, pattern, 0) != 0) {
        return 0;
    }

    int result = (regexec(&regex, str, 0, NULL, 0) == 0);

    regfree(&regex);
    return result;
}


int ok_separador(char c) {
    return match("[(){};,]", c);
}

int ok_operador(char c) {
    return strchr("+-*/=<>", c) != NULL;
}

int ok_indicador(char c) {
    return match("[#@]", c);
}



const char *reservadas[] = {
    "if","else","while","for","return","int","float","char","void","#include","#define", "#ifdef"
};

int total_reservadas = 12;

int ok_reservada(const char *str){
    for(int i=0;i<total_reservadas;i++){
        if(strcmp(str,reservadas[i]) == 0)
            return 1;
    }
    return 0;
}



void registrar(FILE *out, const char *classe, const char *token, int lin, int col){
    fprintf(out,"%-20s %-15s %4d %4d\n",classe,token,lin,col);
}

void analisar_arquivo(FILE *in, FILE *out) {
    int c;
    int linha = 1, coluna = 0, idx;
    char token[MAX_TOKEN];

    fprintf(out,"CLASSE               TOKEN           LIN COL\n");
    fprintf(out,"---------------------------------------------\n");

    while ((c = fgetc(in)) != EOF) {
        coluna++;

        if (c == '\n') {
            linha++;
            coluna = 0;
            continue;
        }

        if (isspace(c))
            continue;

        int col_inicio = coluna;

   
        
if (c == '#' && coluna == 1) {
    idx = 0;
    token[idx++] = c;
    while ((c = fgetc(in)) != EOF && !isspace(c) && c != '\n') {
        token[idx++] = c;
        coluna++;
    }
    token[idx] = '\0';
    if (ok_reservada(token)){
        registrar(out, "PALAVRA_RESERVADA", token, linha, col_inicio);
    }else{
        registrar(out, "ERRO_LEXICO", token, linha, col_inicio);
    }

    
    do {
        c = fgetc(in);
        coluna++;
    } while (isspace(c));

    if (c == '<' || c == '"') {
        char term = (c == '<') ? '>' : '"';
        idx = 0;
        token[idx++] = c;

        int erro = 0;
        while ((c = fgetc(in)) != EOF && c != term) {
            // Marca erro 
            if (!isalnum(c) && c != '.' && c != '_') erro = 1;
            token[idx++] = c;
            coluna++;
        }

        // foi fechada 
        if (c != term) erro = 1;
        if (c == term) token[idx++] = c;

        token[idx] = '\0';

        if (erro)
            registrar(out, "ERRO_LEXICO", token, linha, col_inicio + 8);
        else
            registrar(out, "INDICADOR", token, linha, col_inicio + 8);
    } else if (isalpha(c) || c == '_') { 
   
        idx = 0;
        token[idx++] = c; 
        while ((c = fgetc(in)) != EOF && (isalnum(c) || c == '_')) {
            token[idx++] = c;
            coluna++;
        }
        token[idx] = '\0';
        registrar(out, "INDICADOR", token, linha, col_inicio + 8);
        if (c != EOF) ungetc(c, in);
    }
    continue;
}

       
        if (isdigit(c)) {
            idx = 0;
            int pontos = 0;
            do {
                if (c == '.') pontos++;
                token[idx++] = c;
                c = fgetc(in);
                coluna++;
            } while (c != EOF && (isdigit(c) || c == '.'));

            token[idx] = '\0';
            if (pontos > 1) registrar(out, "ERRO_LEXICO", token, linha, col_inicio);
            else registrar(out, "NUMERAL", token, linha, col_inicio);

            if (c != EOF) ungetc(c, in);
            continue;
        }

        
        if (isalpha(c) || c == '_'/*|| c == '#'*/) {
            idx = 0;
            token[idx++] = c;
            while ((c = fgetc(in)) != EOF && (isalnum(c) || c == '_')) {
                token[idx++] = c;
                coluna++;
            }
            token[idx] = '\0';
            if (ok_reservada(token))
                registrar(out, "PALAVRA_RESERVADA", token, linha, col_inicio);
            else
                registrar(out, "INDICADOR", token, linha, col_inicio);
            if (c != EOF) ungetc(c, in);
            continue;
        }

        
        if (c == '"') {
            idx = 0;
            int fechado = 0;
            while ((c = fgetc(in)) != EOF) {
                if (c == '"') { fechado = 1; break; }
                if (c == '\n') { linha++; coluna = 0; }
                token[idx++] = c;
                coluna++;
            }
            token[idx] = '\0';
            if (fechado)
                registrar(out, "LITERAL", token, linha, col_inicio);
            else
                registrar(out, "ERRO_LEXICO", token, linha, col_inicio);
            continue;
        }

        
        if (c == '/') {
            int prox = fgetc(in);
            if (prox == '/') { 
                idx = 0;
                while ((c = fgetc(in)) != EOF && c != '\n') token[idx++] = c;
                token[idx] = '\0';
                registrar(out, "COMENTARIO", token, linha, col_inicio);
                if (c == '\n') { linha++; coluna = 0; }
                continue;
            } else if (prox == '*') {
                idx = 0;
                int fechado = 0;
                int prev = 0;
                while ((c = fgetc(in)) != EOF) {
                    if (c == '\n') { linha++; coluna = 0; }
                    if (prev == '*' && c == '/') { fechado = 1; break; }
                    prev = c;
                    token[idx++] = c;
                }
                token[idx] = '\0';
                if (fechado)
                    registrar(out, "COMENTARIO", token, linha, col_inicio);
                else
                    registrar(out, "ERRO_LEXICO", token, linha, col_inicio);
                continue;
            } else { 
                registrar(out, "OPERADOR", "/", linha, col_inicio);
                if (prox != EOF) ungetc(prox, in);
                continue;
            }
        }

        
        if (ok_separador(c)) {
            token[0] = c; token[1] = '\0';
            registrar(out, "SEPARADOR", token, linha, col_inicio);
            continue;
        }

        
        if (ok_operador(c)) {
            token[0] = c; token[1] = '\0';
            registrar(out, "OPERADOR", token, linha, col_inicio);
            continue;
        }

       
        if (ok_indicador(c)) {
            token[0] = c; token[1] = '\0';
            registrar(out, "INDICADOR", token, linha, col_inicio);
            continue;
        }

        
        idx = 0;
        token[idx++] = c;
        while ((c = fgetc(in)) != EOF && !isalnum(c) && !isspace(c) &&
               !ok_operador(c) && !ok_separador(c) && !ok_indicador(c)) {
            token[idx++] = c;
            coluna++;
        }
        token[idx] = '\0';
        registrar(out, "ERRO_LEXICO", token, linha, col_inicio);
        if (c != EOF) ungetc(c, in);
    }
}