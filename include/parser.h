#pragma once
#include <iostream>
#include <unordered_map>
#include "lexer.h"
#include "ast.h"
#include "error.h"

class Parser{
public:
    Lexer& lexer;
    Token current_token;
    std::unordered_map<std::string, Token> global_variables;
    
    Parser(Lexer&);
    
    void error(ErrorCode, TokenType, Token);
    void unsupported_operation_error(TokenType, Op*);
    void check_char_operation(AST*, AST*, Op*);
    void eat(TokenType);
    Token get_next_token();
    AST* term();
    AST* expr();
    AST* or_expr();
    AST* and_expr();
    AST* xor_expr();
    AST* comparison();
    AST* additive();
    AST* factor();
    AST* program();
    Block* block();
    std::vector<AST*> declarations();
    std::vector<Param*> formal_parameter_list();
    std::vector<Param*> formal_parameters();
    std::vector<VarDecl*> variable_declarations();
    Type* type_spec();
    Compound* compound_statement();
    std::vector<AST*> statement_list();
    AST* statement();
    Assign* assignment_statement();
    AST* variable();
    AST* procedure_call_statement();
    AST* if_statement();
    AST* if_statement_tail();
    AST* while_loop();
    AST* for_loop();
    AST* empty();
    AST* parse();
};