#include "symbol.h"

using namespace std;

Symbol::Symbol(string n): name(n), type("None"){};
Symbol::Symbol(string n, string t): name(n), type(t){};
BuiltInSymbol::BuiltInSymbol(string n): Symbol(n){};
VarSymbol::VarSymbol(string n, string t): Symbol(n, t){};
ProcedureSymbol::ProcedureSymbol(string n): Symbol(n){};

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
}

void SemanticAnalyzer::error(ErrorCode error_code, Token token) {
    std::cerr << EtoS(error_code) << " -> " << token.toString() << "\n";
    std::abort();
}


void SemanticAnalyzer::visit(AST* node){
    if (node == nullptr){
        throw runtime_error("visit() received a nullptr");
    }
    
    if (auto binOp = dynamic_cast<BinaryOp*>(node)){
        visitBinaryOp(binOp);
    } else if (auto unOp = dynamic_cast<UnaryOp*>(node)){
        visitUnaryOp(unOp);
    } else if (auto program = dynamic_cast<Program*>(node)){
        visitProgram(program);
    } else if (auto compound = dynamic_cast<Compound*>(node)){
        visitCompound(compound);
    } else if (auto assign = dynamic_cast<Assign*>(node)){
        visitAssign(assign);
    } else if (auto noOp = dynamic_cast<NoOp*>(node)){
        visitNoOp(noOp);
    } else if (auto block = dynamic_cast<Block*>(node)){
        visitBlock(block);        
    } else if (auto varDecl = dynamic_cast<VarDecl*>(node)){
        visitVarDecl(varDecl);
    } else if (auto var = dynamic_cast<Var*>(node)){
        visitVar(var);
    } else if (auto num = dynamic_cast<Num*>(node)){
        visitNum(num);
    } else if (auto procedureDeclaration = dynamic_cast<ProcedureDeclaration*>(node)){
        visitProcedureDeclaration(procedureDeclaration);
    } else if (auto procedureCall = dynamic_cast<ProcedureCall*>(node)){
        visitProcedureCall(procedureCall);
    } else {
        throw runtime_error("unsupported node type in 'visit'.");
    }
}

void SemanticAnalyzer::visitProgram(Program* node){
    ScopedSymbolTable* global_scope = new ScopedSymbolTable("Global", 1, current_scope);
    current_scope = global_scope;
    cout << "Entered the global scope\n";
    visit(node->block);
    cout << global_scope->toString() << "\n";
    cout << "Leaving the global scope\n";
    current_scope = current_scope->enclosing_scope;
}

void SemanticAnalyzer::visitBlock(Block* node){
    for (AST* decl : node->declarations){
        visit(decl);
    }
    visit(node->compound_statement);
}

void SemanticAnalyzer::visitVarDecl(VarDecl* node){
    Token var_token = node->var->token;
    string var_name = var_token.value;
    string var_type = TtoS(node->type->type);
    VarSymbol* symbol = new VarSymbol(var_name, var_type);

    if (current_scope->symbol_dict.count(var_name) > 0){
        error(ErrorCode::DUPLICATE_ID, var_token);
    } else {
        current_scope->define(symbol);
    }
}

void SemanticAnalyzer::visitAssign(Assign* node){
    visit(node->left); 
    visit(node->right);
}

void SemanticAnalyzer::visitVar(Var* node){
    Token token = node->token;
    string name = token.value;

    Symbol* symbol = current_scope->lookup(name); 
    if (!symbol){
        error(ErrorCode::ID_NOT_FOUND, token);
    }
}

void SemanticAnalyzer::visitNum(Num* node){

}

void SemanticAnalyzer::visitNoOp(NoOp* node){

}

void SemanticAnalyzer::visitBinaryOp(BinaryOp* node){
    visit(node->left);
    visit(node->right);
}

void SemanticAnalyzer::visitUnaryOp(UnaryOp* node){
    visit(node->expr);
}

void SemanticAnalyzer::visitCompound(Compound* node){
    for (AST* child : node->children){
        visit(child);
    }
}

void SemanticAnalyzer::visitProcedureDeclaration(ProcedureDeclaration* node){
    string procedure_name = node->name;
    ProcedureSymbol* procedure_symbol = new ProcedureSymbol(procedure_name);
    current_scope->define(procedure_symbol);

    int level = current_scope->level + 1;
    ScopedSymbolTable* procedure_scope = new ScopedSymbolTable(procedure_name, level, current_scope);
    cout << "Entering " + procedure_name + "\n";
    current_scope = procedure_scope;

    for (Param* param : node->formal_params){
        Token token = param->var->token;
        string param_name = token.value;
        string param_type = TtoS(param->type->type);
        Symbol* symbol = new VarSymbol(param_name, param_type);
        current_scope->define(symbol);
        procedure_symbol->params.push_back(param);
    }

    visit(node->block);
    cout << procedure_scope->toString() << '\n';
    cout << "Leaving " + procedure_name + "\n";
    current_scope = current_scope->enclosing_scope;
}

void SemanticAnalyzer::visitProcedureCall(ProcedureCall* node){
    string procedure_name = node->name;
    ProcedureSymbol* procedure_symbol = dynamic_cast<ProcedureSymbol*>(current_scope->lookup(procedure_name));
    if (procedure_symbol == nullptr){
        error(ErrorCode::ID_NOT_FOUND, node->token);
    }
    int actual_size = size(procedure_symbol->params);
    int given_size = size(node->given_params);

    if (actual_size != given_size){
        error(ErrorCode::INCORRECT_NUMBER_OF_ARGUMENTS, node->token);
    }

    for (AST* param : node->given_params){
        visit(param);
    }
}