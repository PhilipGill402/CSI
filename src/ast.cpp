#include "ast.h"

using namespace std;

Integer::Integer(int v): value(v){};
Real::Real(float v): value(v){};
Op::Op(string v): value(v){};
BinaryOp::BinaryOp(Num* l, Op* o, Num* r): left(l), op(o), right(r){};
