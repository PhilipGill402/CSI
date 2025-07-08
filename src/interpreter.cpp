#include "interpreter.h"

using namespace std;

Num* Interpreter::visitBinaryOp(AST* node){
    BinaryOp* expression = dynamic_cast<BinaryOp*>(node);
    if (expression == nullptr){
        throw invalid_argument("Incorrect AST node given to 'visitBinaryOp'"); 
    }

    Num* left = expression->left;
    Op* op = expression->op;
    Num* right = expression->right;
    bool isReal = false;
    if (dynamic_cast<Real*>(left) && dynamic_cast<Real*>(right)){
        left = dynamic_cast<Real*>(left);
        right = dynamic_cast<Real*>(right);
        isReal = true;
    } else if (dynamic_cast<Real*>(left)){
        left = dynamic_cast<Real*>(left);
        right = dynamic_cast<Integer*>(right);
        isReal = true;
    } else if (dynamic_cast<Real*>(right)){
        left = dynamic_cast<Integer*>(left);
        right = dynamic_cast<Real*>(right);
        isReal = true;
    } else {
        left = dynamic_cast<Integer*>(left);
        right = dynamic_cast<Integer*>(right);
    }

    if (op->value == "+"){
        if (isReal){
            return new Real(left->value + right->value);
        } else {
            return new Integer(left->value + right->value);
        }
    } else if (op->value == "-"){
        if (isReal){
            return new Real(left->value - right->value);
        } else {
            return new Integer(left->value - right->value);
        }
    } else if (op->value == "*"){
        if (isReal){
            return new Real(left->value * right->value);
        } else {
            return new Integer(left->value * right->value);
        }
    } else if (op->value == "/"){
        if (right->value == 0){
            throw invalid_argument("Cannot divide by zero");
        }
        
        return new Real(left->value / right->value);
        
    } else if (op->value == "INT_DIV"){
        if (right->value == 0){
            throw invalid_argument("Cannot divide by zero");
        }

        return new Integer(left->value / right->value);
    } else {
        throw invalid_argument("Invalid operator type in 'visitBinaryOp'");
    }
    
}