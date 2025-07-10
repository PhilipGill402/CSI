#pragma once
#include <stdexcept>
#include <iostream>
#include "ast.h"

class Interpreter{
public:
    AST* root;
    AST* visit(AST*);
    int visitInteger(Integer*);
    double visitReal(Real*);
    Num* visitBinaryOp(BinaryOp*);

};