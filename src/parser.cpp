#include <parser.h>
#include <interpreter.h>

using namespace std;

Parser::Parser(Lexer& l) : lexer(l), current_token(lexer.get_next_token()) {}

void Parser::eat(TokenType type){
    if (current_token.type == type){
        current_token = lexer.get_next_token();
    }
    else{
        cout << current_token.toString() << '\n';
        cout << TtoS(type) << '\n';
        throw invalid_argument("Token type and the current token do not match");
    }
}

Token Parser::get_next_token(){
    return lexer.get_next_token();
}

AST* Parser::term(){
    AST* node = factor();

    while (current_token.type == TokenType::MUL || current_token.type == TokenType::DIV || current_token.type == TokenType::INT_DIV){ 
        Op* op = new Op(current_token.value); 
        if (current_token.type == TokenType::MUL){
            eat(TokenType::MUL);
        }else if (current_token.type == TokenType::DIV){
            eat(TokenType::DIV);
        }else if (current_token.type == TokenType::INT_DIV){
            eat(TokenType::INT_DIV);
        }

        node = new BinaryOp(node, op, factor());
    }
    
    return node;
}

AST* Parser::factor(){
    if (current_token.type == TokenType::INTEGER_CONST){
        int value = stoi(current_token.value);
        eat(TokenType::INTEGER_CONST);
        Integer* node = new Integer(value);
        return node;
    } else if (current_token.type == TokenType::REAL_CONST){
        double value = stod(current_token.value);
        eat(TokenType::REAL_CONST); 
        Real* node = new Real(value);
        return node;
    } else if (current_token.type == TokenType::LPAREN){
        eat(LPAREN);
        AST* node = expr();
        eat(RPAREN);
        return node;
    } else if (current_token.type == TokenType::ADD){
        Op* op = new Op(current_token.value);
        eat(ADD);
        AST* node = new UnaryOp(op, factor());
        return node;
    } else if (current_token.type == TokenType::SUB){
        Op* op = new Op(current_token.value);
        eat(SUB);
        AST* node = new UnaryOp(op, factor());
        return node;
    } else if (current_token.type == TokenType::ID){
        return variable();
    } else {
        throw runtime_error("Unexpected token in factor(): " + current_token.value);
    }
}

AST* Parser::expr(){
    AST* node = term();
    
    while (current_token.type == TokenType::ADD || current_token.type == TokenType::SUB){
        Op* op = new Op(current_token.value);
        if (current_token.type == TokenType::ADD){
            eat(TokenType::ADD);
        }else{
            eat(TokenType::SUB);
        }
        
        node = new BinaryOp(node, op, term());
    }
    
    return node; 
}

AST* Parser::program(){
    eat(PROGRAM);
    string program_name = current_token.value;
    eat(ID); 
    eat(SEMI);
    Block* block_node = dynamic_cast<Block*>(block()); 
    eat(DOT);
    
    Program* node = new Program(program_name, block_node);
    return node;
}

AST* Parser::block(){
    vector<VarDecl*> decls = declarations();
    Compound* compound = dynamic_cast<Compound*>(compound_statement());

    Block* node = new Block(decls, compound);

    return node;
}

vector<VarDecl*> Parser::declarations(){
    vector<VarDecl*> decls = {};
    if (current_token.type == TokenType::VAR){
        eat(VAR);
        do
        {
            vector<VarDecl*> nodes = variable_declarations(); 
            decls.insert(decls.end(), nodes.begin(), nodes.end());
            eat(SEMI);
        } while (current_token.type == TokenType::ID);
    } 
    
    return decls;
}

vector<VarDecl*> Parser::variable_declarations(){
    vector<Var*> var_nodes = {new Var(current_token)};
    eat(ID);
    while (current_token.type == TokenType::COMMA){
        eat(COMMA);
        var_nodes.push_back(new Var(current_token));
        eat(ID);
    }
    eat(COLON);
    Type* type = type_spec();

    vector<VarDecl*> var_decl_nodes = {};
    for (Var* node : var_nodes){
        var_decl_nodes.push_back(new VarDecl(node, type));
    }

    return var_decl_nodes;
}

Type* Parser::type_spec(){
    TokenType type;
    if (current_token.type == TokenType::INTEGER){
        type = TokenType::INTEGER; 
        eat(INTEGER);
    } else if (current_token.type == TokenType::REAL){
        type = TokenType::REAL;
        eat(REAL);
    } else {
        throw runtime_error("Unrecognized type in type_spec()");
    }

    return new Type(type);
}

AST* Parser::compound_statement(){
    eat(BEGIN);
    vector<AST*> statementList = statement_list();
    eat(END);
    Compound* node = new Compound(statementList);

    return node;
}

vector<AST*> Parser::statement_list(){
    AST* node = statement();
    vector<AST*> children = {node};

    while (current_token.type == TokenType::SEMI){
        eat(SEMI);
        children.push_back(statement());
    }

    return children;
}

AST* Parser::statement(){
    AST* node;
    if (current_token.type == BEGIN){
        //compound statement
        node = compound_statement();
    } else if (current_token.type == ID){
        //assignment statement
        node = assignment_statement();
    } else {
        //empty
        node = empty();
    }

    return node;
}

AST* Parser::assignment_statement(){
    AST* left = variable(); 
    Token token = current_token;
    eat(ASSIGN);
    AST* right = expr();

    return new Assign(left, token, right);
}

AST* Parser::variable(){
    Token token = current_token;
    eat(ID);

    return new Var(token);
}

AST* Parser::empty(){
    return new NoOp();
}

AST* Parser::parse(){
    AST* node = program();

    return node;
}