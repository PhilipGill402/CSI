#pragma once
#include <string>
#include <vector>
#include <lexer.h>
#include <unordered_map>

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
    AST* left;
    Op* op;
    AST* right;

    BinaryOp(AST*, Op*, AST*);
};

class UnaryOp : public AST{
public:
    Op* op;    
    AST* expr;

    UnaryOp(Op*, AST*);
};

class Program : public AST{};

class Compound : public AST{
public:
    std::vector<AST*> children = {};
    Compound(std::vector<AST*>&);
};

class Assign : public AST{
public:
    AST* left;
    Token token;
    AST* right;

    Assign(AST*, Token, AST*);
};

class Var : public AST{
public:
    Token token;

    Var(Token);
};

class NoOp : public AST{};