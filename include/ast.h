#pragma once
#include <string>

class AST{
public:
    virtual ~AST() = default;
};

class Num : public AST{};

class Integer : public Num{
public:
    int value;
    Integer(int v);
};

class Real : public Num{
public:
    float value;
    Real(float v);
};

class Op : public AST{
public:
    std::string value;
    Op(std::string v);
};

class BinaryOp : public AST{
public:
    Num* left;
    Op* op;
    Num* right;

    BinaryOp(Num* l, Op* o, Num* r);
};