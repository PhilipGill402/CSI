#pragma once
#include <stdexcept>
#include <iostream>
#include "ast.h"

class Interpreter{
public:
    AST* root;
    Num* visitBinaryOp(AST*);

};