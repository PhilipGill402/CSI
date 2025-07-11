#include "interpreter.h"

using namespace std;

Interpreter::Interpreter(AST* t): tree(t){};

bool isReal(Num* node){
    return dynamic_cast<Real*>(node) != nullptr;
}

AST* Interpreter::visit(AST* node){
    if (node == nullptr){
        throw runtime_error("visit() received a nullptr");
    }
    if (auto num = dynamic_cast<Integer*>(node)){
        return new Integer(visitInteger(num));
    } else if (auto num = dynamic_cast<Real*>(node)){
        return new Real(visitReal(num));
    } else if (auto binOp = dynamic_cast<BinaryOp*>(node)){
        return visitBinaryOp(binOp);
    } else if (auto unOp = dynamic_cast<UnaryOp*>(node)){
        return visitUnaryOp(unOp);
    } else if (auto program = dynamic_cast<Program*>(node)){
        return visitProgram(program);
    } else if (auto compound = dynamic_cast<Compound*>(node)){
        return visitCompound(compound);
    } else if (auto assign = dynamic_cast<Assign*>(node)){
        return visitAssign(assign);
    } else if (auto var = dynamic_cast<Var*>(node)){
        return visitVar(var);
    } else if (auto noOp = dynamic_cast<NoOp*>(node)){
        return visitNoOp(noOp);
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
    if (!node->left || !node->right) {
        throw runtime_error("BinaryOp has null left or right child");
    }

    Num* left = dynamic_cast<Num*>(visit(node->left));
    Op* op = node->op;
    Num* right = dynamic_cast<Num*>(visit(node->right));

    if (left == nullptr || right == nullptr){
        throw runtime_error("Either left or right needs to be of type Num* in visitBinaryOp()");
    }

    bool isLeftReal = isReal(left);
    bool isRightReal = isReal(right);

    double result = 0.0; 

    if (op->value == "+"){
        result = left->value + right->value;
    } else if (op->value == "-"){
        result = left->value - right->value;
    } else if (op->value == "*"){
        result = left->value * right->value;
    } else if (op->value == "/"){
        result = left->value / right->value;
    } else if (op->value == "INT_DIV"){
        int leftVal = static_cast<int>(left->value);
        int rightVal = static_cast<int>(right->value);
        result = static_cast<int>(leftVal / rightVal);
        return new Integer(result);
    } else {
        throw invalid_argument("Invalid operator type in 'visitBinaryOp'");
    }

    if (isLeftReal || isRightReal){
        return new Real(result);
    }
    return new Integer(static_cast<int>(result));
    
}

Num* Interpreter::visitUnaryOp(UnaryOp* node){
    if (!node->expr || !node->op){
        throw runtime_error("Null argument given to visitUnaryOp()");
    }

    Op* op = node->op;
    Num* newNode = dynamic_cast<Num*>(visit(node->expr));
    if (newNode == nullptr){
        throw runtime_error("Non Num expression passed into UnaryOp");
    }    

    
    if (op->value == "+"){
        newNode->value = +newNode->value; 
    } else if (op->value == "-"){
        newNode ->value = -newNode->value;
    }
    
    if (isReal(newNode)){
        Real* result = dynamic_cast<Real*>(newNode);
        return result;
    }
    Integer* result = dynamic_cast<Integer*>(newNode);
    return result;
}

AST* Interpreter::visitProgram(Program* node){
    return new NoOp();
}

AST* Interpreter::visitCompound(Compound* node){
    for (AST* child : node->children){
        visit(child);
    }

    return new NoOp();
}

AST* Interpreter::visitAssign(Assign* node){
    Var* var = dynamic_cast<Var*>(node->left);
    if (var == nullptr){
        throw runtime_error("Invalid AST* node given to the left of the Assign node");
    }
    Token token = var->token;
    string var_name = token.value;
    Num* var_value = dynamic_cast<Num*>(visit(node->right));
    if (var_value == nullptr){
        throw runtime_error("Invalid AST* node given to the right of the Assign node");
    }
    global_variables[var_name] = var_value;

    return new NoOp();
}

AST* Interpreter::visitVar(Var* node){
    Token token = node->token;
    string var_name = token.value;

    return global_variables[var_name];
}

AST* Interpreter::visitNoOp(NoOp* node){
    return new NoOp();
}

void Interpreter::interpret(){
    visit(tree);
}