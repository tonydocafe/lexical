#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"
#include "lexical.h"

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
// --- REGRAS GRAMATICAIS ---

void declaracao() {
    
    match_tx("PALAVRA_RESERVADA", NULL); 
    match_tx("INDICADOR", NULL);        
    if (strcmp(atual().lexema, "[") == 0) {
        match_tx("SEPARADOR", "[");
        match_tx("NUMERAL", NULL);
        match_tx("SEPARADOR", "]");
    }
    match_tx("SEPARADOR", ";");
}

void comando_if() {
    match_tx("PALAVRA_RESERVADA", "if");
    match_tx("SEPARADOR", "(");
    
    match_tx("INDICADOR", NULL);
    match_tx("OPERADOR", NULL);
    if (strcmp(atual().classe, "OPERADOR") == 0) match_tx("OPERADOR", NULL); 
    match_tx("NUMERAL", NULL);
    match_tx("SEPARADOR", ")");
    
    match_tx("SEPARADOR", "{");
    // Aqui viriam os comandos internos do IF
    while (strcmp(atual().lexema, "}") != 0) posi++; 
    match_tx("SEPARADOR", "}");

    if (strcmp(atual().lexema, "else") == 0) {
        match_tx("PALAVRA_RESERVADA", "else");
        match_tx("SEPARADOR", "{");
        while (strcmp(atual().lexema, "}") != 0) posi++;
        match_tx("SEPARADOR", "}");
    }
}

void comandos() {
    char* lex = atual().lexema;
    char* cls = atual().classe;

    if (strcmp(lex, "if") == 0) {
        comando_if();
    } else if (strcmp(lex, "for") == 0) {
        regra_for();
    } else if (strcmp(lex, "int") == 0 || strcmp(lex, "char") == 0) {
        declaracao();
    } else if (strcmp(lex, "return") == 0) {
        match_tx("PALAVRA_RESERVADA", "return");
        match_tx("NUMERAL", NULL);
        match_tx("SEPARADOR", ";");
    } else if (strcmp(cls, "PALAVRA_RESERVADA") == 0 || strcmp(cls, "INDICADOR") == 0) {
        // Trata printf, scanf e atribuições pulando até o próximo ';'
        while (posi < total_tokens && strcmp(atual().lexema, ";") != 0) {
            posi++;
        }
        match_tx("SEPARADOR", ";");
    } else {
        posi++; 
    }
}


void regra_for() {
    match_tx("PALAVRA_RESERVADA", "for");
    match_tx("SEPARADOR", "(");
    
    if (strcmp(atual().lexema, "int") == 0) match_tx("PALAVRA_RESERVADA", "int");
    match_tx("INDICADOR", NULL); 
    match_tx("OPERADOR", "=");    
    match_tx("NUMERAL", NULL);   
    match_tx("SEPARADOR", ";");   

    match_tx("INDICADOR", NULL); 
    match_tx("OPERADOR", NULL);   
    if (strcmp(atual().classe, "OPERADOR") == 0) match_tx("OPERADOR", NULL); 
    match_tx("NUMERAL", NULL);   
    match_tx("SEPARADOR", ";");

    match_tx("INDICADOR", NULL); 
    match_tx("OPERADOR", "+");    
    match_tx("OPERADOR", "+");    
    
    match_tx("SEPARADOR", ")");
    match_tx("SEPARADOR", "{");
    

    while (posi < total_tokens && strcmp(atual().lexema, "}") != 0) {
        comandos(); 
    }
    
    match_tx("SEPARADOR", "}");
}



void programa() {
    
    while (strcmp(atual().lexema, "#include") == 0) {
        match_tx("PALAVRA_RESERVADA", "#include");
        match_tx("INDICADOR", NULL);
    }

    
    match_tx("PALAVRA_RESERVADA", "int");
    match_tx("INDICADOR", "main");
    match_tx("SEPARADOR", "(");
    match_tx("SEPARADOR", ")");
    match_tx("SEPARADOR", "{");

    
    while (strcmp(atual().lexema, "}") != 0 && posi < total_tokens) {
        char* lex = atual().lexema;
        
        if (strcmp(lex, "int") == 0 || strcmp(lex, "char") == 0) {
            declaracao();
        } 
        else if (strcmp(lex, "if") == 0) {
            comando_if();
        } 
        else if (strcmp(lex, "for") == 0) { 
            regra_for();
        } 
        else if (strcmp(lex, "return") == 0) {
            match_tx("PALAVRA_RESERVADA", "return");
            match_tx("NUMERAL", NULL);
            match_tx("SEPARADOR", ";");
        } 
        else {
           
            posi++; 
        }
    }
    match_tx("SEPARADOR", "}");
}