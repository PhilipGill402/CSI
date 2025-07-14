#pragma once
#include <stdexcept>
#include <iostream>
#include "ast.h"
#include "activation_record.h"
#include "symbol.h"

class Interpreter{
public:
    CallStack call_stack = CallStack(); 
    AST* tree;

    Interpreter(AST*);

    AST* visit(AST*);
    int visitInteger(Integer*);
    double visitReal(Real*);
    Num* visitBinaryOp(BinaryOp*);
    Num* visitUnaryOp(UnaryOp*);
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
    void interpret();
};