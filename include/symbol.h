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
    void visit(AST* node);
    void visitBinaryOp(BinaryOp* node);
    void visitUnaryOp(UnaryOp* node);
    void visitProgram(Program* node);
    void visitCompound(Compound* node);
    void visitAssign(Assign* node);
    void visitBlock(Block* node);
    void visitVarDecl(VarDecl* node);
    void visitVar(Var* node);
    void visitNoOp(NoOp* node);
    void visitNum(Num* node);
    void visitProcedureDeclaration(ProcedureDeclaration* node);
    void visitProcedureCall(ProcedureCall* node);
};