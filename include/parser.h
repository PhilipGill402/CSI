#pragma once
#include <iostream>
#include <unordered_map>
#include "lexer.h"
#include "ast.h"

class Parser{
public:
    Lexer& lexer;
    Token current_token;
    std::unordered_map<std::string, Token> global_variables;
    
    Parser(Lexer&);
    
    void eat(TokenType);
    Token get_next_token();
    AST* term();
    AST* expr();
    AST* factor();
    AST* program();
    AST* compound_statement();
    std::vector<AST*> statement_list();
    AST* statement();
    AST* assignment_statement();
    AST* variable();
    AST* empty();
    AST* parse();
};