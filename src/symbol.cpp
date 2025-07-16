#include "symbol.h"

using namespace std;

Symbol::Symbol(string n): name(n), type("None"){};
Symbol::Symbol(string n, string t): name(n), type(t){};
BuiltInSymbol::BuiltInSymbol(string n): Symbol(n){};
VarSymbol::VarSymbol(string n, string t): Symbol(n, t){};
ProcedureSymbol::ProcedureSymbol(string n): Symbol(n){};
EmptySymbol::EmptySymbol():Symbol("NONE"){};

string Symbol::toString(){
    string str = "<";
    str += name + ", " + type + ">";

    return str;
}

ScopedSymbolTable::ScopedSymbolTable(string n, int l): name(n), level(l), enclosing_scope(nullptr){
    if (level == 0){
        add_builtins();
    }

};

ScopedSymbolTable::ScopedSymbolTable(string n, int l, ScopedSymbolTable* s): name(n), level(l), enclosing_scope(s){};

void ScopedSymbolTable::define(Symbol* symbol){
    symbol_dict[symbol->name] = symbol;
}

Symbol* ScopedSymbolTable::lookup(string name){
    if (symbol_dict.count(name) > 0){
        return symbol_dict.at(name);
    }
    if (enclosing_scope != nullptr){
        return enclosing_scope->lookup(name);
    } else {
        throw runtime_error("Symbol '" + name + "' not found.");
    }

}

string ScopedSymbolTable::toString(){
    string str = "Name: " + name + "\n";
    str += "Level: " + to_string(level) + "\n";
    str += "Enclosing Scope: " + enclosing_scope->name + "\n";

    for (auto entry : symbol_dict){
        str += entry.first + ": " + entry.second->toString() + "\n";
    }

    return str;
}

void ScopedSymbolTable::add_builtins(){
    define(new BuiltInSymbol("INTEGER"));
    define(new BuiltInSymbol("REAL"));
    define(new BuiltInSymbol("BOOLEAN"));
    define(new BuiltInSymbol("CHAR"));
    define(new BuiltInSymbol("STRING"));
}

void SemanticAnalyzer::error(ErrorCode error_code, Token token) {
    cerr << EtoS(error_code) << " -> " << token.toString() << "\n";
    abort();
}

void SemanticAnalyzer::incorrect_type_error(string expected, string received, Symbol* symbol){
    cerr << "Expected type of: " + expected + " for symbol " + symbol->toString() + " Received type of: " + received + "\n";
    abort();
}

Symbol* SemanticAnalyzer::visit(AST* node){
    if (node == nullptr){
        throw runtime_error("visit() received a nullptr");
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
        throw runtime_error("Unrecognized type given"); 
    } else {
        throw runtime_error("unsupported node type in 'visit'.");
    }
}

Symbol* SemanticAnalyzer::visitProgram(Program* node){
    ScopedSymbolTable* global_scope = new ScopedSymbolTable("Global", 1, current_scope);
    current_scope = global_scope;
    visit(node->block);
    cout << current_scope->toString() << "\n";
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
    string var_name = var_token.value;
    string var_type = TtoS(node->type->type);
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
    if (var_symbol->type != given_symbol->name){
        incorrect_type_error(var_symbol->type, given_symbol->name, var_symbol);
    }

    return new EmptySymbol();
}

Symbol* SemanticAnalyzer::visitVar(Var* node){
    Token token = node->token;
    string name = token.value;

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

    if (left->name == "INTEGER" && right->name == "INTEGER"){
        if (node->op->value == "/"){
            return new BuiltInSymbol("REAL");
        } 
        return new BuiltInSymbol("INTEGER");
    } else if (left->name == "REAL" || right->name == "REAL"){
        if (node->op->value == "DIV"){
            return new BuiltInSymbol("INTEGER");
        } 
        return new BuiltInSymbol("REAL");
    } else if (left->name == "BOOLEAN" && right->name == "BOOLEAN"){
        return new BuiltInSymbol("BOOLEAN");
    } else if (left->name == "CHAR" && right->name == "CHAR"){
        return new BuiltInSymbol("CHAR");
    } else if (left->name == "STRING" && right->name == "STRING"){
        return new BuiltInSymbol("STRING");
    } else {
        incorrect_type_error(left->name, right->name, left);
        throw new runtime_error("Incorrect Type Error");
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
    string procedure_name = node->name;
    ProcedureSymbol* procedure_symbol = new ProcedureSymbol(procedure_name);
    current_scope->define(procedure_symbol);

    int level = current_scope->level + 1;
    ScopedSymbolTable* procedure_scope = new ScopedSymbolTable(procedure_name, level, current_scope);
    current_scope = procedure_scope;

    for (Param* param : node->formal_params){
        Token token = param->var->token;
        string param_name = token.value;
        string param_type = TtoS(param->type->type);
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
    string procedure_name = node->name;
    ProcedureSymbol* procedure_symbol = dynamic_cast<ProcedureSymbol*>(current_scope->lookup(procedure_name));
    if (procedure_symbol == nullptr){
        error(ErrorCode::ID_NOT_FOUND, node->token);
    }
    node->procedure_symbol = procedure_symbol;
    int actual_size = size(procedure_symbol->params);
    int given_size = size(node->given_params);

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