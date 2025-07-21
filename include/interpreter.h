#pragma once
#include <stdexcept>
#include <iostream>
#include "ast.h"
#include "activation_record.h"
#include "symbol.h"
#include "helpers.h"

class Interpreter{
public:
    CallStack call_stack = CallStack();
    std::vector<FunctionDeclaration*> functions;
    AST* tree;

    Interpreter(AST*);

    AST* visit(AST*);
    int visitInteger(Integer*);
    double visitReal(Real*);
    bool visitBoolean(Boolean*);
    char visitChar(Char*);
    std::string visitString(String*);
    Value* visitBinaryOp(BinaryOp*);
    Value* visitNumBinaryOp(BinaryOp*);
    Boolean* visitBoolBinaryOp(BinaryOp*);
    String* visitStringBinaryOp(BinaryOp*);
    Value* visitUnaryOp(UnaryOp*);
    AST* visitProgram(Program*);
    AST* visitCompound(Compound*);
    AST* visitAssign(Assign*);
    AST* visitVar(Var*);
    AST* visitNoOp(NoOp*);
    AST* visitBlock(Block*);
    AST* visitVarDecl(VarDecl*);
    AST* visitType(Type*);
    AST* visitProcedureDeclaration(ProcedureDeclaration*);
    AST* visitProcedureCall(ProcedureCall*);
    AST* visitIfStatement(IfStatement*);
    AST* visitWhileLoop(WhileLoop*);
    AST* visitForLoop(ForLoop*);
    AST* visitRepeatUntil(RepeatUntil*);
    AST* visitFunctionDeclaratin(FunctionDeclaration*);
    Value* visitFunctionCall(FunctionCall*);
    void interpret();

    //builtins
    void builtin_writeln(ProcedureCall*);
    void builtin_write(ProcedureCall*);
    void builtin_readln(ProcedureCall*);
    void builtin_read(ProcedureCall*);
    int builtin_length(ProcedureCall*);
};