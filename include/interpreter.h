#pragma once
#include <stdexcept>
#include <iostream>
#include "ast.h"

class Interpreter{
public:
    std::unordered_map<std::string, Num*> global_variables;   
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
    void interpret();
};