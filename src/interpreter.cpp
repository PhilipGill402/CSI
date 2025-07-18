#include "interpreter.h"

Interpreter::Interpreter(AST* t): tree(t){};

bool isReal(Value* node){
    return dynamic_cast<Real*>(node) != nullptr;
}

AST* Interpreter::visit(AST* node){
    if (node == nullptr){
        throw std::runtime_error("visit() received a nullptr");
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
    } else if (auto block = dynamic_cast<Block*>(node)){
        return visitBlock(block);        
    } else if (auto varDecl = dynamic_cast<VarDecl*>(node)){
        return visitVarDecl(varDecl);
    } else if (auto type = dynamic_cast<Type*>(node)){
        return visitType(type);
    } else if (auto procedure_declaration = dynamic_cast<ProcedureDeclaration*>(node)){
        return visitProcedureDeclaration(procedure_declaration);
    } else if (auto procedure_call = dynamic_cast<ProcedureCall*>(node)){
        return visitProcedureCall(procedure_call);
    } else if (auto boolean = dynamic_cast<Boolean*>(node)){
        return new Boolean(visitBoolean(boolean));
    } else if (auto character = dynamic_cast<Char*>(node)){
        return new Char(visitChar(character));
    } else if (auto str = dynamic_cast<String*>(node)){
        return new String(visitString(str));
    } else if (auto if_statement = dynamic_cast<IfStatement*>(node)){
        return visitIfStatement(if_statement);
    } else if (auto while_loop = dynamic_cast<WhileLoop*>(node)){
        return visitWhileLoop(while_loop);
    } else if (auto for_loop = dynamic_cast<ForLoop*>(node)){
        return visitForLoop(for_loop);
    } else {
        throw std::runtime_error("unsupported node type in 'visit'.");
    }
}

int Interpreter::visitInteger(Integer* node){
    return node->value;
}

double Interpreter::visitReal(Real* node){
    return node->value;
}

bool Interpreter::visitBoolean(Boolean* node){
    return node->value;
}

char Interpreter::visitChar(Char* node){
    return node->value;
}

std::string Interpreter::visitString(String* node){
    return node->value;
}

Value* Interpreter::visitBinaryOp(BinaryOp* node){
    if (!node->left || !node->right) {
        throw std::runtime_error("BinaryOp has null left or right child");
    }

    if (dynamic_cast<Num*>(visit(node->left)) && dynamic_cast<Num*>(visit(node->right))){
        return visitNumBinaryOp(node);
    } else if (dynamic_cast<Boolean*>(visit(node->left)) && dynamic_cast<Boolean*>(visit(node->right))){
        return visitBoolBinaryOp(node);
    } else if (dynamic_cast<String*>(visit(node->left)) && dynamic_cast<String*>(visit(node->right))){
        return visitStringBinaryOp(node);
    } else {
        throw std::runtime_error("Unsupported type in 'visitBinaryOp'");
    }
}

Value* Interpreter::visitNumBinaryOp(BinaryOp* node){
    Num* left = dynamic_cast<Num*>(visit(node->left));
    Op* op = node->op;
    Num* right = dynamic_cast<Num*>(visit(node->right));

    if (left == nullptr || right == nullptr){
        throw std::runtime_error("Either left or right needs to be of type Num* in visitBinaryOp()");
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
        result = static_cast<double>(left->value) / right->value;
        return new Real(result);
    } else if (op->value == "DIV"){
        int leftVal = static_cast<int>(left->value);
        int rightVal = static_cast<int>(right->value);
        result = static_cast<int>(leftVal / rightVal);
        return new Integer(result);
    } else if (op->value == "="){
        bool bool_result = left->value == right->value;
        return new Boolean(bool_result);
    } else if (op->value == "!="){
        bool bool_result = left->value != right->value;
        Boolean* node = new Boolean(bool_result);
        return node;
    } else if (op->value == "<"){
        bool bool_result = left->value < right->value;
        return new Boolean(bool_result);
    } else if (op->value == ">"){
        bool bool_result = left->value > right->value;
        return new Boolean(bool_result);
    } else if (op->value == "<="){
        bool bool_result = left->value <= right->value;
        return new Boolean(bool_result);
    } else if (op->value == ">="){
        bool bool_result = left->value >= right->value;
        return new Boolean(bool_result);
    } else {
        throw std::invalid_argument("Invalid operator type for type 'Num'");
    }

    if (isLeftReal || isRightReal){
        return new Real(result);
    }
    return new Integer(static_cast<int>(result));
}

Boolean* Interpreter::visitBoolBinaryOp(BinaryOp* node){
    Boolean* left = dynamic_cast<Boolean*>(visit(node->left));
    Op* op = node->op; 
    Boolean* right = dynamic_cast<Boolean*>(visit(node->right));
    bool result;
   
    if (op->value == "AND"){
        result = left->value && right->value;
    } else if (op->value == "OR"){
        result = left->value || right->value;
    } else if (op->value == "XOR") {
        result = left->value != right->value;
    } else if (op->value == "="){
        result = left->value == right->value;
    } else if (op->value == "!="){
        result = left->value != right->value;
    } else if (op->value == "<"){
        result = left->value < right->value;
    } else if (op->value == ">"){
        result = left->value > right->value;
    } else if (op->value == "<="){
        result = left->value <= right->value;
    } else if (op->value == ">="){
        result = left->value >= right->value;
    } else {
        throw std::invalid_argument("Invalid operator type for type 'Boolean'");
    }


    return new Boolean(result);
}

String* Interpreter::visitStringBinaryOp(BinaryOp* node){
    String* left = dynamic_cast<String*>(visit(node->left));
    Op* op = node->op;
    String* right = dynamic_cast<String*>(visit(node->right));
    std::string result;

    if (op->value == "+"){
        result = left->value + right->value;
    } else {
        throw std::invalid_argument("Invalid operator type for type 'String'");
    }

    return new String(result);
}

Value* Interpreter::visitUnaryOp(UnaryOp* node){
    if (!node->expr || !node->op){
        throw std::runtime_error("Null argument given to visitUnaryOp()");
    }

    Op* op = node->op;
    Num* newNode = dynamic_cast<Num*>(visit(node->expr));
    if (newNode == nullptr){
        throw std::runtime_error("Non Num expression passed into UnaryOp");
    }    

    if (op->value == "+"){
        double original = static_cast<double>(newNode->value);
        original = +original;

        if (isReal(newNode)){
            return new Real(original);
        }
        int new_original = static_cast<int>(original); 
        return new Integer(new_original);
    
    } else if (op->value == "-"){
        double original = static_cast<double>(newNode->value);
        original = -original;

        if (isReal(newNode)){
            return new Real(original);
        }
        int new_original = static_cast<int>(original);

        return new Integer(new_original);

    } else if (op->value == "NOT"){
        bool original = static_cast<bool>(newNode->value); 
        return new Boolean(!original);

    } else {
        throw std::runtime_error("Received unsupported unary operation type");
    }
    
    
}

AST* Interpreter::visitProgram(Program* node){
    std::string program_name = node->program_name;
    ActivationRecord program_ar = ActivationRecord(program_name, ARType::AR_PROGRAM, 1);
    call_stack.records.push(program_ar);
    AST* new_node = visit(node->block);
    ActivationRecord ar = call_stack.records.top();
    std::cout << ar.toString();
    call_stack.records.pop();
    return new_node;
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
        throw std::runtime_error("Invalid AST* node given to the left of the Assign node");
    }
    Token token = var->token;
    std::string var_name = token.value;
    Value* var_value = dynamic_cast<Value*>(visit(node->right));
    if (var_value == nullptr){
        throw std::runtime_error("Invalid AST* node given to the right of the Assign node");
    }
    call_stack.records.top().members[var_name] = var_value;

    return new NoOp();
}

AST* Interpreter::visitVar(Var* node){
    Token token = node->token;
    std::string var_name = token.value;
    Value* var = call_stack.records.top().members.at(var_name);
    return var;
}

AST* Interpreter::visitNoOp(NoOp* node){
    return new NoOp();
}

AST* Interpreter::visitBlock(Block* node){
    for (AST* decl : node->declarations){
        visit(decl);
    }

    visit(node->compound_statement);

    return new NoOp();
}

AST* Interpreter::visitVarDecl(VarDecl* node){
    return new NoOp();
}

AST* Interpreter::visitType(Type* node){
    return new NoOp();
}

AST* Interpreter::visitProcedureDeclaration(ProcedureDeclaration* node){
    return new NoOp();
}

AST* Interpreter::visitProcedureCall(ProcedureCall* node){
    std::string procedure_name = node->name;
    int level = call_stack.records.top().level + 1;
    ActivationRecord procedure_ar = ActivationRecord(procedure_name, ARType::AR_PROCEDURE, level);
    std::vector<Param*> formal_arguments = node->procedure_symbol->params;
    std::vector<AST*> given_arguments = node->given_params;

    for (int idx = 0; idx < given_arguments.size(); idx++){
        std::string name = formal_arguments[idx]->var->token.value;
        Num* value = dynamic_cast<Num*>(visit(given_arguments[idx]));
        if (value == nullptr){
            throw std::runtime_error("Given argument couldn't be converted to type Num*");
        }
        procedure_ar.members[name] = value;
    }

    call_stack.records.push(procedure_ar);
    visit(node->procedure_symbol->block);
    call_stack.records.pop();
    return new NoOp();
}

AST* Interpreter::visitIfStatement(IfStatement* node){
    Boolean* conditional_node = dynamic_cast<Boolean*>(visit(node->conditional));
    if (conditional_node == nullptr){
        throw std::runtime_error("Boolean expected in conditional");
    }
    bool conditional = conditional_node->value;
    if (conditional){
        visit(node->if_statement); 
    } else {
        visit(node->else_statement);
    }

    return new NoOp();
}

AST* Interpreter::visitWhileLoop(WhileLoop* node){
    Boolean* conditional_node = dynamic_cast<Boolean*>(visit(node->conditional));
    if (conditional_node == nullptr){
        throw std::runtime_error("Boolean expected in conditional");
    }
    bool conditional = conditional_node->value;
    while (conditional){
        visit(node->statement);
        conditional_node = dynamic_cast<Boolean*>(visit(node->conditional));
        if (conditional_node == nullptr){
            throw std::runtime_error("Boolean expected in conditional");
        }
        conditional = conditional_node->value;
    }

    return new NoOp(); 
}

AST* Interpreter::visitForLoop(ForLoop* node){
    visit(node->assignment);
    Var* var = dynamic_cast<Var*>(node->assignment->left);
    std::string var_name = var->token.value;
    Integer* start = dynamic_cast<Integer*>(call_stack.records.top().members[var_name]);
    Integer* current = new Integer(start->value);
    Integer* end = dynamic_cast<Integer*>(visit(node->target));
    if (end == nullptr){
        throw std::runtime_error("End value must evaluate to type 'Integer'");
    }
    Integer* increment = node->increment;

    if (increment->value == 1){
        for (int i = start->value; i <= end->value; i++){
            Assign* assign = new Assign(var, Token(TokenType::ASSIGN, ":=", -1,-1), current);
            visit(assign);
            current->value++;
            visit(node->body);
        }
    } else {
        for (int i = start->value; i >= end->value; i--){
            Assign* assign = new Assign(var, Token(TokenType::ASSIGN, ":=", -1,-1), current);
            visit(assign); 
            current->value--; 
            visit(node->body);
        }
    }
    

    return new NoOp();
}

void Interpreter::interpret(){
    visit(tree);
}