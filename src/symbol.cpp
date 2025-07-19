#include "symbol.h"

Symbol::Symbol(std::string n): name(n), type(n){};
Symbol::Symbol(std::string n, std::string t): name(n), type(t){};
BuiltInSymbol::BuiltInSymbol(std::string n): Symbol(n){};
VarSymbol::VarSymbol(std::string n, std::string t): Symbol(n, t){};
ProcedureSymbol::ProcedureSymbol(std::string n): Symbol(n){};
EmptySymbol::EmptySymbol():Symbol("NONE"){};

std::string Symbol::toString(){
    std::string str = "<";
    str += name + ", " + type + ">";

    return str;
}

ScopedSymbolTable::ScopedSymbolTable(std::string n, int l): name(n), level(l), enclosing_scope(nullptr){
    if (level == 0){
        add_builtins();
    }

};

ScopedSymbolTable::ScopedSymbolTable(std::string n, int l, ScopedSymbolTable* s): name(n), level(l), enclosing_scope(s){};

void ScopedSymbolTable::define(Symbol* symbol){
    symbol_dict[symbol->name] = symbol;
}

Symbol* ScopedSymbolTable::lookup(std::string name){
    if (symbol_dict.count(name) > 0){
        return symbol_dict.at(name);
    }
    if (enclosing_scope != nullptr){
        return enclosing_scope->lookup(name);
    } else {
        throw std::runtime_error("Symbol '" + name + "' not found.");
    }

}

std::string ScopedSymbolTable::toString(){
    std::string str = "Name: " + name + "\n";
    str += "Level: " + std::to_string(level) + "\n";
    str += "Enclosing Scope: " + enclosing_scope->name + "\n";

    for (auto entry : symbol_dict){
        str += entry.first + ": " + entry.second->toString() + "\n";
    }

    return str;
}

void ScopedSymbolTable::add_builtins(){
    define(new BuiltInSymbol("NUM")); 
    define(new BuiltInSymbol("INTEGER"));
    define(new BuiltInSymbol("REAL"));
    define(new BuiltInSymbol("BOOLEAN"));
    define(new BuiltInSymbol("CHAR"));
    define(new BuiltInSymbol("STRING"));
}

void SemanticAnalyzer::error(ErrorCode error_code, Token token) {
    std::cerr << EtoS(error_code) << " -> " << token.toString() << "\n";
    abort();
}

void SemanticAnalyzer::incorrect_type_error(std::string expected, std::string received, Symbol* symbol){
    std::cerr << "Expected type of: " + expected + " for symbol " + symbol->toString() + " Received type of: " + received + "\n";
    abort();
}

Symbol* SemanticAnalyzer::visit(AST* node){
    if (node == nullptr){
        throw std::runtime_error("visit() received a nullptr");
    }
    
    if (auto binOp = dynamic_cast<BinaryOp*>(node)){
        return visitBinaryOp(binOp);
    } else if (auto unOp = dynamic_cast<UnaryOp*>(node)){
        return visitUnaryOp(unOp);
    } else if (auto program = dynamic_cast<Program*>(node)){
        return visitProgram(program);
    } else if (auto compound = dynamic_cast<Compound*>(node)){
        return visitCompound(compound);
    } else if (auto assign = dynamic_cast<Assign*>(node)){
        return visitAssign(assign);
    } else if (auto noOp = dynamic_cast<NoOp*>(node)){
        return visitNoOp(noOp);
    } else if (auto block = dynamic_cast<Block*>(node)){
        return visitBlock(block);        
    } else if (auto varDecl = dynamic_cast<VarDecl*>(node)){
        return visitVarDecl(varDecl);
    } else if (auto var = dynamic_cast<Var*>(node)){
        return visitVar(var);
    } else if (auto procedureDeclaration = dynamic_cast<ProcedureDeclaration*>(node)){
        return visitProcedureDeclaration(procedureDeclaration);
    } else if (auto procedureCall = dynamic_cast<ProcedureCall*>(node)){
        return visitProcedureCall(procedureCall);
    } else if (auto integer = dynamic_cast<Integer*>(node)){
        return visitInteger(integer);
    } else if (auto real = dynamic_cast<Real*>(node)){
        return visitReal(real);
    } else if (auto boolean = dynamic_cast<Boolean*>(node)){
        return visitBoolean(boolean);
    } else if (auto character = dynamic_cast<Char*>(node)){
        return visitChar(character);
    } else if (auto string = dynamic_cast<String*>(node)){
        return visitString(string);
    } else if (auto num = dynamic_cast<Value*>(node)){
        throw std::runtime_error("Unrecognized type given"); 
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

Symbol* SemanticAnalyzer::visitProgram(Program* node){
    ScopedSymbolTable* global_scope = new ScopedSymbolTable("Global", 1, current_scope);
    current_scope = global_scope;
    visit(node->block);
    std::cout << current_scope->toString() << "\n";
    current_scope = current_scope->enclosing_scope;

    return new EmptySymbol();
}

Symbol* SemanticAnalyzer::visitBlock(Block* node){
    for (AST* decl : node->declarations){
        visit(decl);
    }
    visit(node->compound_statement);

    return new EmptySymbol();
}

Symbol* SemanticAnalyzer::visitVarDecl(VarDecl* node){
    Token var_token = node->var->token;
    std::string var_name = var_token.value;
    std::string var_type = TtoS(node->type->type);
    VarSymbol* symbol = new VarSymbol(var_name, var_type);

    if (current_scope->symbol_dict.count(var_name) > 0){
        error(ErrorCode::DUPLICATE_ID, var_token);
    } else {
        current_scope->define(symbol);
    }

    return new EmptySymbol();
}

Symbol* SemanticAnalyzer::visitAssign(Assign* node){
    Symbol* var_symbol = visit(node->left); 
    Symbol* given_symbol = visit(node->right);
    if ((var_symbol->type == "INTEGER" && given_symbol->name == "NUM") || (var_symbol->type == "REAL" && given_symbol->name == "NUM")){
        return new EmptySymbol();
    } else if (var_symbol->type != given_symbol->type){
        incorrect_type_error(var_symbol->type, given_symbol->type, var_symbol);
    }

    return new EmptySymbol();
}

Symbol* SemanticAnalyzer::visitVar(Var* node){
    Token token = node->token;
    std::string name = token.value;

    Symbol* symbol = current_scope->lookup(name); 
    if (!symbol){
        error(ErrorCode::ID_NOT_FOUND, token);
    }

    return symbol;
}

Symbol* SemanticAnalyzer::visitNum(Num* node){
    return new EmptySymbol();
}

Symbol* SemanticAnalyzer::visitNoOp(NoOp* node){
    return new EmptySymbol();
}

Symbol* SemanticAnalyzer::visitBinaryOp(BinaryOp* node){
    Symbol* left = visit(node->left);
    Symbol* right = visit(node->right);
    std::string op = node->op->value;

    if (left->type == "INTEGER" && right->type == "INTEGER"){
        if (op == "=" || op == "!=" || op == ">" || op == ">=" || op == "<" || op == "<=" || op == "AND" || op == "OR"){
            return new BuiltInSymbol("BOOLEAN");
        }

        return new BuiltInSymbol("NUM");
    } else if (left->type == "REAL" || right->type == "REAL"){
        if (op == "=" || op == "!=" || op == ">" || op == ">=" || op == "<" || op == "<=" || op == "AND" || op == "OR"){
            return new BuiltInSymbol("BOOLEAN");
        }
        return new BuiltInSymbol("NUM");
    } else if (left->type == "BOOLEAN" && right->type == "BOOLEAN"){
        return new BuiltInSymbol("BOOLEAN");
    } else if (left->type == "CHAR" && right->type == "CHAR"){
        return new BuiltInSymbol("CHAR");
    } else if (left->type == "STRING" && right->type == "STRING"){
        return new BuiltInSymbol("STRING");
    } else {

        incorrect_type_error(left->type, right->type , left);
        throw std::runtime_error("Incorrect Type Error");
    }
}

Symbol* SemanticAnalyzer::visitUnaryOp(UnaryOp* node){
    visit(node->expr);

    return new EmptySymbol();
}

Symbol* SemanticAnalyzer::visitCompound(Compound* node){
    for (AST* child : node->children){
        visit(child);
    }

    return new EmptySymbol();
}

Symbol* SemanticAnalyzer::visitProcedureDeclaration(ProcedureDeclaration* node){
    std::string procedure_name = node->name;
    ProcedureSymbol* procedure_symbol = new ProcedureSymbol(procedure_name);
    current_scope->define(procedure_symbol);

    int level = current_scope->level + 1;
    ScopedSymbolTable* procedure_scope = new ScopedSymbolTable(procedure_name, level, current_scope);
    current_scope = procedure_scope;

    for (Param* param : node->formal_params){
        Token token = param->var->token;
        std::string param_name = token.value;
        std::string param_type = TtoS(param->type->type);
        Symbol* symbol = new VarSymbol(param_name, param_type);
        current_scope->define(symbol);
        procedure_symbol->params.push_back(param);
    }
    procedure_symbol->block = node->block;
    visit(node->block);
    current_scope = current_scope->enclosing_scope;

    return new EmptySymbol();
}

Symbol* SemanticAnalyzer::visitProcedureCall(ProcedureCall* node){
    std::string procedure_name = node->name;
    ProcedureSymbol* procedure_symbol = dynamic_cast<ProcedureSymbol*>(current_scope->lookup(procedure_name));
    if (procedure_symbol == nullptr){
        error(ErrorCode::ID_NOT_FOUND, node->token);
    }
    node->procedure_symbol = procedure_symbol;
    int actual_size = procedure_symbol->params.size();
    int given_size = node->given_params.size();

    if (actual_size != given_size){
        error(ErrorCode::INCORRECT_NUMBER_OF_ARGUMENTS, node->token);
    }

    for (AST* param : node->given_params){
        visit(param);
    }

    return new EmptySymbol();
}

Symbol* SemanticAnalyzer::visitInteger(Integer* node){
    return new BuiltInSymbol("INTEGER");
}

Symbol* SemanticAnalyzer::visitReal(Real* node){
    return new BuiltInSymbol("REAL");
}

Symbol* SemanticAnalyzer::visitBoolean(Boolean* node){
    return new BuiltInSymbol("BOOLEAN");
}

Symbol* SemanticAnalyzer::visitChar(Char* node){
    return new BuiltInSymbol("CHAR");
}

Symbol* SemanticAnalyzer::visitString(String* node){
    return new BuiltInSymbol("STRING");
}

Symbol* SemanticAnalyzer::visitIfStatement(IfStatement* node){
    Symbol* conditional_var = visit(node->conditional);
    if (conditional_var->type != "BOOLEAN"){
        throw std::runtime_error("Expected a 'BOOLEAN' conditional"); 
    }

    visit(node->if_statement);
    visit(node->else_statement);

    return new EmptySymbol();
}

Symbol* SemanticAnalyzer::visitWhileLoop(WhileLoop* node){
    Symbol* conditional_var = visit(node->conditional);
    if (conditional_var->type != "BOOLEAN"){
        throw std::runtime_error("Expected a 'BOOLEAN' conditional");
    }

    visit(node->statement);

    return new EmptySymbol();
}

Symbol* SemanticAnalyzer::visitForLoop(ForLoop* node){
    Var* counter_var = dynamic_cast<Var*>(node->assignment->left);
    std::string counter_name = counter_var->token.value;
    visit(node->assignment);
    visit(node->target);

    for (AST* child : node->statements){
        if (auto assign = dynamic_cast<Assign*>(child)){
            Var* var = dynamic_cast<Var*>(assign->left);
            if (var && var->token.value == counter_name){
                throw std::runtime_error("Cannot assign to loop variable '" + counter_name + "' inside a for loop");
            }
        }
    }

    return new EmptySymbol();
}