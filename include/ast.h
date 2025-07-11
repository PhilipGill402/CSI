#pragma once
#include <string>
#include <vector>
#include <lexer.h>
#include <unordered_map>

class Block;
class Compound;
class VarDecl;
class Type;
class Var;
class Token;

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

class Program : public AST{
public:
    std::string program_name;
    Block* block;
    Program(std::string, Block*);
};

class Block : public AST{
public:
    std::vector<VarDecl*> declarations;
    Compound* compound_statement;

    Block(std::vector<VarDecl*>&, Compound*);
};

class VarDecl : public AST{
public:
    Var* var;
    Type* type;

    VarDecl(Var*, Type*);
};

class Type : public AST{
public:
    TokenType type;

    Type(TokenType);
};

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