#pragma once
#include <string>
#include <vector>
#include <lexer.h>
#include <unordered_map>

class ProcedureSymbol;
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
    Num(char);
    
    virtual std::string toString() const = 0; 
    virtual ~Num() = default;
};

class Integer : public Num{
public:
    Integer(int);
    std::string toString() const override;
};

class Real : public Num{
public:
    Real(double);
    std::string toString() const override;
};

class Boolean : public Num{
public:
    Boolean(bool);
    std::string toString() const override;
};

class Char : public Num{
public:
    Char(char);
    std::string toString() const override;
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
    std::vector<AST*> declarations;
    Compound* compound_statement;

    Block(std::vector<AST*>&, Compound*);
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

class Param : public AST{
public:
    Var* var;
    Type* type;

    Param(Var*, Type*);
};

class ProcedureDeclaration : public AST{
public:
    std::string name;
    std::vector<Param*> formal_params;
    Block* block;

    ProcedureDeclaration(std::string, std::vector<Param*>&, Block*);
};

class ProcedureCall : public AST{
public:
    std::string name;
    std::vector<AST*> given_params;
    Token token;
    ProcedureSymbol* procedure_symbol;

    ProcedureCall(std::string, std::vector<AST*>&, Token);
};

class NoOp : public AST{};