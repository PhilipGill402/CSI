#pragma once
#include <iostream>
#include "lexer.h"
#include "ast.h"

class Parser{
public:
    Lexer& lexer;
    Token current_token;
    
    Parser(Lexer&);
    
    void eat(TokenType);
    Token get_next_token();
    AST* term();
    AST* expr();
    AST* factor();

};