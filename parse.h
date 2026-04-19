#ifndef PARSER_H
#define PARSER_H

#include "lexical.h"
#include "ast.h"

AST* programa();
AST* comandos();
AST* declaracao();
AST* comando_if();
AST* regra_for();



#endif