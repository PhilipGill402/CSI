#include "ast.h"

using namespace std;

Num::Num(int v): value(v){};
Num::Num(double v): value(v){};
Integer::Integer(int v): Num(v){};
Real::Real(double v): Num(v){};
Op::Op(string v): value(v){};
BinaryOp::BinaryOp(AST* l, Op* o, AST* r): left(l), op(o), right(r){};
UnaryOp::UnaryOp(Op* o, AST* e): op(o), expr(e){};
Compound::Compound(vector<AST*>& c): children(c){};
Assign::Assign(AST* l, Token t, AST* r): left(l), token(t), right(r){};
Var::Var(Token t): token(t){};
