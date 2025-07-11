#include "ast.h"

using namespace std;

Num::Num(int v): value(v){};
Num::Num(double v): value(v){};
Integer::Integer(int v): Num(v){};
Real::Real(double v): Num(v){};
Op::Op(string v): value(v){};
BinaryOp::BinaryOp(AST* l, Op* o, AST* r): left(l), op(o), right(r){};
UnaryOp::UnaryOp(Op* o, AST* e): op(o), expr(e){};
Program::Program(string n, Block* b): program_name(n), block(b){};
Block::Block(vector<VarDecl*>& v, Compound* c): declarations(v), compound_statement(c){};
VarDecl::VarDecl(Var* v, Type* t): var(v), type(t){};
Type::Type(TokenType t): type(t){};
Compound::Compound(vector<AST*>& c): children(c){};
Assign::Assign(AST* l, Token t, AST* r): left(l), token(t), right(r){};
Var::Var(Token t): token(t){};
