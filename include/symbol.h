#pragma once
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <iostream>
#include "ast.h"

class Symbol{
public:
    std::string name;
    std::string type;

    Symbol(std::string);
    Symbol(std::string, std::string);

    std::string toString();

    virtual ~Symbol() {}
};

class BuiltInSymbol : public Symbol{
public:
    BuiltInSymbol(std::string);
};

class VarSymbol : public Symbol{
public:
    VarSymbol(std::string, std::string);
};

class ProcedureSymbol : public Symbol{
public:
    std::vector<Param*> params;
    Block* block;

    ProcedureSymbol(std::string);
};

class EmptySymbol : public Symbol{
public:
    EmptySymbol();
};

class ScopedSymbolTable{
public:
    std::string name;
    int level;
    std::unordered_map<std::string, Symbol*> symbol_dict = {};
    ScopedSymbolTable* enclosing_scope;

    ScopedSymbolTable(std::string, int);
    ScopedSymbolTable(std::string, int, ScopedSymbolTable*);
    void add_builtins();
    void define(Symbol*);
    Symbol* lookup(std::string);
    std::string toString();
};

class SemanticAnalyzer{
public:
    ScopedSymbolTable* current_scope = new ScopedSymbolTable("Builtins", 0);

    void error(ErrorCode, Token);
    void incorrect_type_error(std::string, std::string, Symbol*);
    Symbol* visit(AST* node);
    Symbol* visitBinaryOp(BinaryOp* node);
    Symbol* visitUnaryOp(UnaryOp* node);
    Symbol* visitProgram(Program* node);
    Symbol* visitCompound(Compound* node);
    Symbol* visitAssign(Assign* node);
    Symbol* visitBlock(Block* node);
    Symbol* visitVarDecl(VarDecl* node);
    Symbol* visitVar(Var* node);
    Symbol* visitNoOp(NoOp* node);
    Symbol* visitNum(Num* node);
    Symbol* visitProcedureDeclaration(ProcedureDeclaration* node);
    Symbol* visitProcedureCall(ProcedureCall* node);
    Symbol* visitInteger(Integer* node);
    Symbol* visitReal(Real* node);
    Symbol* visitBoolean(Boolean* node);
    Symbol* visitChar(Char* node);
    Symbol* visitString(String* node);
    Symbol* visitIfStatement(IfStatement* node);
    Symbol* visitWhileLoop(WhileLoop* node);
    Symbol* visitForLoop(ForLoop* node);
};