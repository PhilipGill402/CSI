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

class Value : public AST{
public:
    virtual std::string toString() const = 0;
    virtual ~Value() = default;
};

class Num : public Value{
public:
    double value;
    bool isReal = false;
    Num(double);
    
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

class Boolean : public Value{
public:
    bool value;
    Boolean(bool);
    std::string toString() const override;
};

class Char : public Value{
public:
    char value;
    Char(char);
    std::string toString() const override;
};

class String : public Value{
public:
    std::string value;
    String(std::string);
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

class IfStatement : public AST{
public:
    AST* conditional;
    AST* if_statement;
    AST* else_statement;

    IfStatement(AST*, AST*, AST*); 
};

class WhileLoop : public AST{
public:
    AST* conditional;
    AST* statement;

    WhileLoop(AST*, AST*);
};

class NoOp : public AST{};