#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"
#include "lexical.h"
#include "ast.h"


void conectar(AST *pai, AST *filho) {
    if (pai && filho && pai->n_filhos < 10) {
        pai->filhos[pai->n_filhos++] = filho;
    }
}



int posi = 0;

Token atual() {
    if (posi < total_tokens) return tokens[posi];
    Token fim = {"FIM", "EOF", 0, 0};
    return fim;
}

void match_tx(const char* classe, const char* lexema) {
    Token t = atual();
    int erro = 0;

   
    if (strcmp(t.classe, classe) != 0) erro = 1;
    if (lexema != NULL && strcmp(t.lexema, lexema) != 0) erro = 1;

    if (erro) {
        printf("\n[ERRO SINTATICO] Linha %d, Col %d: Esperado %s '%s', mas encontrou %s '%s'\n",
               t.linha, t.coluna, classe, lexema ? lexema : "---", t.classe, t.lexema);
        exit(1);
    }
    posi++; 
}


AST* declaracao() {
    AST *no = novo_no("DECLARACAO", "");
    
    
    conectar(no, novo_no("TIPO", atual().lexema));
    match_tx("PALAVRA_RESERVADA", NULL);
    
   
    conectar(no, novo_no("VARIAVEL", atual().lexema));
    match_tx("INDICADOR", NULL);
    
    
    if (strcmp(atual().lexema, "[") == 0) {
        match_tx("SEPARADOR", "[");
        conectar(no, novo_no("TAMANHO", atual().lexema));
        match_tx("NUMERAL", NULL);
        match_tx("SEPARADOR", "]");
    }
    
    match_tx("SEPARADOR", ";");
    return no;
}


AST* comando_if() {
    AST *no = novo_no("COMANDO_IF", "");
    
    match_tx("PALAVRA_RESERVADA", "if");
    match_tx("SEPARADOR", "(");
    
    
    AST *cond = novo_no("CONDICAO", "");
    conectar(cond, novo_no("VARIAVEL", atual().lexema));
    match_tx("INDICADOR", NULL);
    
    conectar(cond, novo_no("OPERADOR", atual().lexema));
    match_tx("OPERADOR", NULL);
    if (strcmp(atual().classe, "OPERADOR") == 0) {
        conectar(cond, novo_no("OPERADOR", atual().lexema));
        match_tx("OPERADOR", NULL);
    }
    
    conectar(cond, novo_no("VALOR", atual().lexema));
    match_tx("NUMERAL", NULL);
    conectar(no, cond);
    
    match_tx("SEPARADOR", ")");
    match_tx("SEPARADOR", "{");
    
    
    AST *entao = novo_no("ENTAO", "");
    while (strcmp(atual().lexema, "}") != 0) {
        conectar(entao, comandos()); 
    }
    conectar(no, entao);
    match_tx("SEPARADOR", "}");

    
    if (strcmp(atual().lexema, "else") == 0) {
        match_tx("PALAVRA_RESERVADA", "else");
        match_tx("SEPARADOR", "{");
        AST *senao = novo_no("SENAO", "");
        while (strcmp(atual().lexema, "}") != 0) {
            conectar(senao, comandos());
        }
        conectar(no, senao);
        match_tx("SEPARADOR", "}");
    }

    return no;
}


AST* comandos() {
    char* lex = atual().lexema;
    if (strcmp(lex, "if") == 0) return comando_if();
    if (strcmp(lex, "for") == 0) return regra_for();
    if (strcmp(lex, "int") == 0 || strcmp(lex, "char") == 0) return declaracao();
    
    
    AST *func = novo_no("FUNCAO", lex);
    while (posi < total_tokens && strcmp(atual().lexema, ";") != 0) {
        posi++;
    }
    match_tx("SEPARADOR", ";");
    return func;
}


AST* regra_for() {
    AST *no = novo_no("LOOP_FOR", "");
    match_tx("PALAVRA_RESERVADA", "for");
    match_tx("SEPARADOR", "(");
    
    
    AST *init = novo_no("INIT", "");
    while(strcmp(atual().lexema, ";") != 0) {
        conectar(init, novo_no("TOKEN", atual().lexema));
        posi++;
    }
    conectar(no, init);
    match_tx("SEPARADOR", ";");

    
    while(strcmp(atual().lexema, "{") != 0) posi++;
    
    match_tx("SEPARADOR", "{");
    AST *corpo = novo_no("CORPO_FOR", "");
    while (strcmp(atual().lexema, "}") != 0) {
        conectar(corpo, comandos());
    }
    conectar(no, corpo);
    match_tx("SEPARADOR", "}");
    
    return no;
}



AST* programa() {
    AST *raiz = novo_no("RAIZ", "Programa_C");

    
    while (strcmp(atual().lexema, "#include") == 0) {
        AST *inc = novo_no("INCLUDE", atual().lexema);
        posi++; 
        conectar(inc, novo_no("BIBLIOTECA", atual().lexema));
        posi++; 
        conectar(raiz, inc);
    }

   
    match_tx("PALAVRA_RESERVADA", "int");
    match_tx("INDICADOR", "main");
    AST *main_no = novo_no("FUNCAO_PRINCIPAL", "main");
    match_tx("SEPARADOR", "("); match_tx("SEPARADOR", ")");
    match_tx("SEPARADOR", "{");

    while (posi < total_tokens && strcmp(atual().lexema, "}") != 0) {
        conectar(main_no, comandos());
    }
    
    conectar(raiz, main_no);
    match_tx("SEPARADOR", "}");
    return raiz;
}
