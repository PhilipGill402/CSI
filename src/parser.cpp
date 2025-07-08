#include <parser.h>

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
    AST left = factor();

    while (current_token.type == TokenType::MUL || current_token.type == TokenType::DIV || current_token.type == TokenType::INT_DIV){ 
        if (current_token.type == TokenType::MUL){
            Op op = Op("*");
            eat(TokenType::MUL);
            return BinaryOp(&left, &op, &factor());
        }else if (current_token.type == TokenType::DIV){
            Op op = Op("/");
            eat(TokenType::DIV);
        }else if (current_token.type == TokenType::INT_DIV){
            Op op = Op("INT_DIV");
            eat(TokenType::INT_DIV);
        }

        AST right = factor();
        AST node = BinaryOp(&left, &op, &right);
    }

    
}

AST* Parser::factor(){
    if (current_token.type == TokenType::INTEGER_CONST){
        return Integer(stoi(current_token.value));
    }else if (current_token.type == TokenType::REAL_CONST){
        return Real(stof(current_token.value));
    }else if (current_token.type == LPAREN){
        eat(LPAREN);
        AST node = expr();
        eat(RPAREN);

        return node;
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
        node = new BinaryOp(dynamic_cast<Num*>(node), op, dynamic_cast<Num*>(term()));
    }

    return node; 
}