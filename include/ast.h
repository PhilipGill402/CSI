#pragma once
#include <string>

class AST{
public:
    virtual ~AST() = default;
};

class Num : public AST{
public:
    double value;
    Num(int);
    Num(double);
};

class Integer : public Num{
public:
    Integer(int);
};

class Real : public Num{
public:
    Real(double);
};

class Op : public AST{
public:
    std::string value;
    Op(std::string);
};

class BinaryOp : public AST{
public:
    Num* left;
    Op* op;
    Num* right;

    BinaryOp(Num*, Op*, Num*);
};