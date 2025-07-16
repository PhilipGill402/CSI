#include "ast.h"

using namespace std;

Num::Num(double v): value(v){};
Integer::Integer(int v): Num(v){};
Real::Real(double v): Num(v){};
Boolean::Boolean(bool v): value(v){};
Char::Char(char v): value(v){};
String::String(string v): value(v){};
Op::Op(string v): value(v){};
BinaryOp::BinaryOp(AST* l, Op* o, AST* r): left(l), op(o), right(r){};
UnaryOp::UnaryOp(Op* o, AST* e): op(o), expr(e){};
Program::Program(string n, Block* b): program_name(n), block(b){};
Block::Block(vector<AST*>& v, Compound* c): declarations(v), compound_statement(c){};
VarDecl::VarDecl(Var* v, Type* t): var(v), type(t){};
Type::Type(TokenType t): type(t){};
Compound::Compound(vector<AST*>& c): children(c){};
Assign::Assign(AST* l, Token t, AST* r): left(l), token(t), right(r){};
Var::Var(Token t): token(t){};
Param::Param(Var* v, Type* t): var(v), type(t){};
ProcedureDeclaration::ProcedureDeclaration(string n, vector<Param*>& p, Block* b): name(n), formal_params(p), block(b){};
ProcedureCall::ProcedureCall(string n, vector<AST*>& p, Token t): name(n), given_params(p), token(t){};

string Integer::toString() const {
    return to_string(static_cast<int>(value));
}

string Real::toString() const {
    return to_string(value);
}

string Boolean::toString() const {
    return value ? "true" : "false";
}

string Char::toString() const {
    return string(1,static_cast<char>(value));
}

string String::toString() const {
    return value;
}