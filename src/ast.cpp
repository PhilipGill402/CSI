#include "ast.h"

Num::Num(double v): value(v){};
Integer::Integer(int v): Num(v){};
Real::Real(double v): Num(v){};
Boolean::Boolean(bool v): value(v){};
Char::Char(char v): value(v){};
String::String(std::string v): value(v){};
Op::Op(std::string v): value(v){};
BinaryOp::BinaryOp(AST* l, Op* o, AST* r): left(l), op(o), right(r){};
UnaryOp::UnaryOp(Op* o, AST* e): op(o), expr(e){};
Program::Program(std::string n, Block* b): program_name(n), block(b){};
Block::Block(std::vector<AST*>& v, Compound* c): declarations(v), compound_statement(c){};
VarDecl::VarDecl(Var* v, Type* t): var(v), type(t){};
Type::Type(TokenType t): type(t){};
Compound::Compound(std::vector<AST*>& c): children(c){};
Assign::Assign(AST* l, Token t, AST* r): left(l), token(t), right(r){};
Var::Var(Token t): token(t){};
Param::Param(Var* v, Type* t): var(v), type(t){};
ProcedureDeclaration::ProcedureDeclaration(std::string n, std::vector<Param*>& p, Block* b): name(n), formal_params(p), block(b){};
ProcedureCall::ProcedureCall(std::string n, std::vector<AST*>& p, Token t): name(n), given_params(p), token(t){};
IfStatement::IfStatement(AST* c, AST* i, AST* e): conditional(c), if_statement(i), else_statement(e){};
WhileLoop::WhileLoop(AST* c, AST* s): conditional(c), statement(s){};

std::string Integer::toString() const {
    return std::to_string(static_cast<int>(value));
}

std::string Real::toString() const {
    return std::to_string(value);
}

std::string Boolean::toString() const {
    return value ? "true" : "false";
}

std::string Char::toString() const {
    return std::string(1,static_cast<char>(value));
}

std::string String::toString() const {
    return value;
}