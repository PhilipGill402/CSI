#include "interpreter.h"

using namespace std;

AST* Interpreter::visit(AST* node){
    if (auto num = dynamic_cast<Integer*>(node)){
        visitInteger(num);
    } else if (auto num = dynamic_cast<Real*>(node)){
        visitReal(num);
    } else if (auto binOp = dynamic_cast<BinaryOp*>(node)){
        visitBinaryOp(binOp);
    } else {
        throw runtime_error("unsupported node type in 'visit'.");
    }
}

int Interpreter::visitInteger(Integer* node){
    return node->value;
}

double Interpreter::visitReal(Real* node){
    return node->value;
}

Num* Interpreter::visitBinaryOp(BinaryOp* node){
    Num* left = dynamic_cast<Num*>(visit(node->left));
    Op* op = node->op;
    Num* right = dynamic_cast<Num*>(visit(node->right));

    if (left == nullptr || right == nullptr){
        throw runtime_error("Either left or right needs to be of type Num* in visitBinaryOp()");
    }

    if (op->value == "+"){
        return new Num(left + right);
    } else if (op->value == "-"){
        
    } else if (op->value == "*"){
        
    } else if (op->value == "/"){
        
    } else if (op->value == "INT_DIV"){
        
    } else {
        throw invalid_argument("Invalid operator type in 'visitBinaryOp'");
    }
    
}