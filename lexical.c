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
    "if","else","while","for","return","int","float","char","void"
};

int total_reservadas = 9;

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

void analisar_arquivo(FILE *in, FILE *out){

    int c;
    int linha = 1;
    int coluna = 0;
    int idx;

    char token[MAX_TOKEN];

    fprintf(out,"CLASSE               TOKEN           LIN COL\n");
    fprintf(out,"---------------------------------------------\n");

    while((c=fgetc(in))!=EOF){

        coluna++;
        if(c=='\n'){
            linha++;
            coluna=0;
            continue;
        }
        
        if(isspace(c))
            continue;

        int col_inicio = coluna;

        // NUMERAL
        if(isdigit(c)){
            idx=0;
            token[idx++]=c;

            while((c=fgetc(in))!=EOF && isdigit(c)){
                token[idx++]=c;
                coluna++;
            }

            token[idx]='\0';

            registrar(out,"NUMERAL",token,linha,col_inicio);

            if(c!=EOF) ungetc(c,in);
        }

        // IDENTIFICADOR / PALAVRA RESERVADA
        else if(isalpha(c) || c=='_'){
            idx=0;
            token[idx++]=c;

            while((c=fgetc(in))!=EOF && (isalnum(c)||c=='_')){
                token[idx++]=c;
                coluna++;
            }

            token[idx]='\0';

            if(ok_reservada(token))
                registrar(out,"PALAVRA_RESERVADA",token,linha,col_inicio);
            else
                registrar(out,"INDICADOR",token,linha,col_inicio);

            if(c!=EOF) ungetc(c,in);
        }

        // LITERAL (string)
        else if(c=='"'){
            idx=0;

            while((c=fgetc(in))!=EOF && c!='"'){
                token[idx++]=c;
                coluna++;
            }

            token[idx]='\0';

            registrar(out,"LITERAL",token,linha,col_inicio);
        }

        // COMENTARIO //
        else if(c=='/'){
            int prox=fgetc(in);

            if(prox=='/'){
                idx=0;

                while((c=fgetc(in))!=EOF && c!='\n'){
                    token[idx++]=c;
                }

                token[idx]='\0';

                registrar(out,"COMENTARIO",token,linha,col_inicio);

                linha++;
                coluna=0;
            }
            else{
                registrar(out,"OPERADOR","/",linha,col_inicio);
                if(prox!=EOF) ungetc(prox,in);
            }
        }

        // SEPARADOR
        else if(ok_separador(c)){
            token[0]=c;
            token[1]='\0';
            registrar(out,"SEPARADOR",token,linha,col_inicio);
        }

        // OPERADOR
        else if(ok_operador(c)){
            token[0]=c;
            token[1]='\0';
            registrar(out,"OPERADOR",token,linha,col_inicio);
        }

        // INDICADOR
        else if(ok_indicador(c)){
            token[0]=c;
            token[1]='\0';
            registrar(out,"INDICADOR",token,linha,col_inicio);
        } else {
            token[0] = c;
            token[1] = '\0';
            registrar(out,"ERRO_LEXICO",token,linha,col_inicio);
        }
    }
}