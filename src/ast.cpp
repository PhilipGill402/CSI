#include "ast.h"

using namespace std;

Num::Num(int v): value(v){};
Num::Num(double v): value(v){};
Integer::Integer(int v): Num(v){};
Real::Real(double v): Num(v){};
Op::Op(string v): value(v){};
BinaryOp::BinaryOp(Num* l, Op* o, Num* r): left(l), op(o), right(r){};
