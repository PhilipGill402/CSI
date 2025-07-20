#include "parser.h"
#include <interpreter.h>

Parser::Parser(Lexer& l) : lexer(l), current_token(lexer.get_next_token()) {}

void Parser::error(ErrorCode error_code, TokenType expected_token_type, Token received_token){
    std::string msg = EtoS(error_code) + " -> " "Expected: " + TtoS(expected_token_type) + " Received: " + received_token.toString(); 
    std::cerr << msg << "\n";
    abort();
}

void Parser::eat(TokenType type){
    if (current_token.type == type){
        current_token = lexer.get_next_token();
    }
    else{
        error(ErrorCode::UNEXPECTED_TOKEN, type, current_token);
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
        AST* right = factor();
        node = new BinaryOp(node, op, right);
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
    } else if (current_token.type == TokenType::TRUE){
        bool value = true;
        eat(TRUE);
        Boolean* node = new Boolean(value);
        return node;
    } else if (current_token.type == TokenType::FALSE){
        bool value = false;
        eat(FALSE);
        Boolean* node = new Boolean(value);
        return node;
    } else if (current_token.type == TokenType::LPAREN){
        eat(LPAREN);
        AST* node = expr();
        eat(RPAREN);
        return node;
    } else if (current_token.type == TokenType::ADD){
        Op* op = new Op(current_token.value);
        eat(ADD);
        AST* right = factor();
        AST* node = new UnaryOp(op, right);
        return node;
    } else if (current_token.type == TokenType::SUB){
        Op* op = new Op(current_token.value);
        eat(SUB);
        AST* right = factor(); 
        AST* node = new UnaryOp(op, right);
        return node;
    } else if (current_token.type == TokenType::NOT){
        Op* op = new Op(current_token.value);
        eat(NOT);
        AST* right = factor();
        AST* node = new UnaryOp(op, right);
        return node;
    } else if (current_token.type == TokenType::ID){
        return variable();
    } else if (current_token.type == TokenType::STRING_LITERAL){
        String* node = new String(current_token.value);
        eat(STRING_LITERAL);
        return node; 
    } else {
        throw std::runtime_error("Unexpected token in factor(): " + current_token.value);
    }
}

AST* Parser::expr(){ 
    if (current_token.type == TokenType::SINGLE_QUOTE){
        AST* node; 
        eat(SINGLE_QUOTE);
        if (current_token.value.size() > 1){
            error(ErrorCode::UNEXPECTED_TOKEN, TokenType::ID, current_token);
        } 
        node = new Char(current_token.value[0]);
        eat(ID);
        eat(SINGLE_QUOTE); 
        return node; 
    }

    return or_expr();
}

AST* Parser::or_expr(){
    AST* node = and_expr();
    
    while (current_token.type == TokenType::OR){
        eat(OR);
        AST* right = and_expr();
        node = new BinaryOp(node, new Op("OR"), right); 
    }
    
    return node;
}

AST* Parser::and_expr(){
    AST* node = xor_expr();

    while (current_token.type == TokenType::AND){
        eat(AND);
        AST* right = xor_expr();
        node = new BinaryOp(node, new Op("AND"), right);
    }

    return node;
}

AST* Parser::xor_expr(){
    AST* node = comparison();

    while (current_token.type == TokenType::XOR){
        eat(XOR);
        AST* right = comparison();
        node = new BinaryOp(node, new Op("XOR"), right);
    }

    return node;
}

AST* Parser::comparison(){
    AST* node = additive();
    while (current_token.type == EQUAL || current_token.type == NOT_EQUAL || current_token.type == GREATER_THAN || current_token.type == LESS_THAN || current_token.type == GTE || current_token.type == LTE){
        Op* op = new Op(current_token.value); 
        if (current_token.type == EQUAL){
            eat(EQUAL);
        } else if (current_token.type == NOT_EQUAL){
            eat(NOT_EQUAL); 
        } else if (current_token.type == GREATER_THAN){
            eat(GREATER_THAN); 
        } else if (current_token.type == LESS_THAN){
            eat(LESS_THAN); 
        } else if (current_token.type == GTE){
            eat(GTE); 
        } else if (current_token.type == LTE){
            eat(LTE); 
        }

        node = new BinaryOp(node, op, additive());
    }

    return node;
}

AST* Parser::additive(){
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
    std::string program_name = current_token.value;
    eat(ID); 
    eat(SEMI);
    Block* block_node = dynamic_cast<Block*>(block()); 
    eat(DOT);
    
    Program* node = new Program(program_name, block_node);
    return node;
}

Block* Parser::block(){
    std::vector<AST*> decls = declarations();
    Compound* compound = dynamic_cast<Compound*>(compound_statement());

    Block* node = new Block(decls, compound);

    return node;
}

std::vector<AST*> Parser::declarations(){
    std::vector<AST*> decls = {};
    while (current_token.type == TokenType::VAR){
        eat(VAR);
        do
        {
            std::vector<VarDecl*> nodes = variable_declarations(); 
            decls.insert(decls.end(), nodes.begin(), nodes.end());
            eat(SEMI);
        } while (current_token.type == TokenType::ID);
    } 
    while (current_token.type == TokenType::PROCEDURE){
        eat(PROCEDURE);
        std::vector<Param*> formal_params;
        std::string procedure_name = current_token.value;
        eat(ID);
        if (current_token.type == TokenType::LPAREN){
            eat(LPAREN); 
            formal_params = formal_parameter_list();
            eat(RPAREN);
        } 
        eat(SEMI);  
        Block* procedure_block = block();
        eat(SEMI);
        ProcedureDeclaration* procedure_declaration = new ProcedureDeclaration(procedure_name, formal_params, procedure_block);
        decls.push_back(procedure_declaration);
    }
    
    return decls;
}

std::vector<Param*> Parser::formal_parameter_list(){
    std::vector<Param*> parameters = formal_parameters();
    if (current_token.type == TokenType::SEMI){
        eat(SEMI);
        std::vector<Param*> new_parameters = formal_parameter_list();
        parameters.insert(parameters.end(), new_parameters.begin(), new_parameters.end());
    }

    return parameters;
}

std::vector<Param*> Parser::formal_parameters(){
    std::vector<Token> tokens = {current_token};
    eat(ID);
    while(current_token.type == TokenType::COMMA){
        eat(COMMA);
        tokens.push_back(current_token);
        eat(ID);
    }
    eat(COLON);
    Type* type = type_spec();

    std::vector<Param*> formal_params;
    formal_params.reserve(tokens.size());
    
    for (Token token : tokens){
        Var* var = new Var(token);
        Param* param = new Param(var, type);
        formal_params.push_back(param);
    }

    return formal_params;
}

std::vector<VarDecl*> Parser::variable_declarations(){
    std::vector<Var*> var_nodes = {new Var(current_token)};
    eat(ID);
    while (current_token.type == TokenType::COMMA){
        eat(COMMA);
        var_nodes.push_back(new Var(current_token));
        eat(ID);
    }
    eat(COLON);
    Type* type = type_spec();

    std::vector<VarDecl*> var_decl_nodes = {};
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
    } else if (current_token.type == TokenType::BOOLEAN){
        type = TokenType::BOOLEAN;
        eat(BOOLEAN);
    } else if (current_token.type == TokenType::CHAR){
        type = TokenType::CHAR;
        eat(CHAR);
    } else if (current_token.type == TokenType::STRING){
        type = TokenType::STRING;
        eat(STRING);
    } else {
        throw std::runtime_error("Unrecognized type in type_spec()");
    }

    return new Type(type);
}

Compound* Parser::compound_statement(){
    eat(BEGIN);
    std::vector<AST*> statementList = statement_list();
    eat(END);
    Compound* node = new Compound(statementList);

    return node;
}

std::vector<AST*> Parser::statement_list(){
    AST* node = statement();
    std::vector<AST*> children = {node};

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
    } else if (current_token.type == ID && lexer.current_char == '('){
        //assignment statement
        node = procedure_call_statement();
    } else if (current_token.type == ID) {
        node = assignment_statement();
    } else if (current_token.type == IF){
        node = if_statement();
    } else if (current_token.type == WHILE){
        node = while_loop();
    } else if (current_token.type == FOR){
        node = for_loop();
    } else if (current_token.type == REPEAT){
        node = repeat_until();
    } else {
        //empty
        node = empty();
    }

    return node;
}

Assign* Parser::assignment_statement(){
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

AST* Parser::procedure_call_statement(){
    Token token = current_token;
    std::string procedure_name = token.value;
    std::vector<AST*> given_params;
    eat(ID);
    eat(LPAREN);
    
    if (current_token.type != TokenType::RPAREN){
        AST* node = expr();
        given_params.push_back(node);
    }

    while (current_token.type == COMMA){
        eat(COMMA);
        AST* node = expr();
        given_params.push_back(node);
    }

    eat(RPAREN);

    ProcedureCall* procedure_call = new ProcedureCall(procedure_name, given_params, token);

    return procedure_call;
}

AST* Parser::if_statement(){
    eat(IF);
    AST* conditional;
    AST* expression = expr();
    if (auto bool_conditional = dynamic_cast<Boolean*>(expression)){
        conditional = bool_conditional;
    } else if (auto bin_op_conditional = dynamic_cast<BinaryOp*>(expression)){
        conditional = bin_op_conditional;
    } else{
        throw std::runtime_error("Expected boolean value in conditional");
    }
    eat(THEN);

    AST* if_statement;
    if (current_token.type == TokenType::BEGIN){
        if_statement = compound_statement();
    } else {
        if_statement = statement(); 
    }
    
    AST* else_statement;
    if (current_token.type == TokenType::ELSE){
        eat(ELSE);
        else_statement = if_statement_tail(); 
    } else {
        else_statement = empty();
    }
    
    return new IfStatement(conditional, if_statement, else_statement);
}

AST* Parser::if_statement_tail(){
    if (current_token.type == TokenType::IF){
        return if_statement();
    } else {
        if (current_token.type == TokenType::BEGIN){
            return compound_statement();
        } else {
            return statement();
        }
    }
}

AST* Parser::while_loop(){
    eat(WHILE);
    AST* conditional;
    AST* expression = expr();
    if (auto bool_conditional = dynamic_cast<Boolean*>(expression)){
        conditional = bool_conditional;
    } else if (auto bin_op_conditional = dynamic_cast<BinaryOp*>(expression)){
        conditional = bin_op_conditional;
    } else{
        throw std::runtime_error("Expected boolean value in conditional");
    }

    eat(DO);
    AST* loop_statement;
    if (current_token.type == TokenType::BEGIN){
        loop_statement = compound_statement();
    } else {
        loop_statement = statement(); 
    }

    return new WhileLoop(conditional, loop_statement);
}

AST* Parser::for_loop(){
    eat(FOR);
    Assign* assignment = assignment_statement();
    Integer* increment;
    if (current_token.type == TokenType::TO){
        eat(TO);
        increment = new Integer(1); 
    } else if (current_token.type == TokenType::DOWNTO){
        eat(DOWNTO);
        increment = new Integer(-1);
    } else {
        throw std::runtime_error("Expected token 'TO' or 'DOWNTO' but instead received " + current_token.toString());
    }
    AST* target = expr();
    eat(DO);
    AST* statements;
    if (current_token.type == TokenType::BEGIN){
        statements = compound_statement(); 
    } else {
        statements = statement();
    }

    return new ForLoop(assignment, target, increment, statements);
}

AST* Parser::repeat_until(){
    eat(REPEAT);
    std::vector<AST*> statements;
    while (current_token.type != TokenType::UNTIL){
        AST* line = statement();
        statements.push_back(line);
        eat(SEMI);
    }
    eat(UNTIL);
    AST* conditional;
    AST* expression = expr();
    if (auto bool_conditional = dynamic_cast<Boolean*>(expression)){
        conditional = bool_conditional;
    } else if (auto bin_op_conditional = dynamic_cast<BinaryOp*>(expression)){
        conditional = bin_op_conditional;
    } else{
        throw std::runtime_error("Expected boolean value in conditional");
    }
    
    return new RepeatUntil(conditional, statements);
}

AST* Parser::empty(){
    return new NoOp();
}

AST* Parser::parse(){
    AST* node = program();

    return node;
}