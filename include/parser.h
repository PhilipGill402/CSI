#pragma once
#include <iostream>
#include "lexer.h"
#include "ast.h"

class Parser{
public:
    Lexer& lexer;
    Token current_token;
    
    Parser(Lexer& l);
    
    void eat(TokenType type);
    Token get_next_token();
    AST* term();
    AST* expr();
    AST* factor();

};