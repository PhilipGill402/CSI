#include <parser.h>
#include <interpreter.h>

using namespace std;

Parser::Parser(Lexer& l) : lexer(l), current_token(lexer.get_next_token()) {}

void Parser::eat(TokenType type){
    if (current_token.type == type){
        current_token = lexer.get_next_token();
    }
    else{
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
    cout << current_token.toString() << endl;
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
    } else if (current_token.type == LPAREN){
        eat(LPAREN);
        AST* node = expr();
        eat(RPAREN);
        return node;
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