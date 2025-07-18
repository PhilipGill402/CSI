#include "lexer.h"

Lexer::Lexer(std::string txt): text(txt), pos(0), lineno(1), column(1), current_char(text.empty() ? '\0' : txt[0]){};

void Lexer::error(){
    std::string msg = "Lexer error on '" + std::string(1, current_char) + "' line: " + std::to_string(lineno) + " column: " + std::to_string(column);
    std::cerr << msg << "\n";
    abort();
}

std::string toUpper(std::string str){
    std::string newStr; 
    for (char c : str){
        newStr += toupper(c);
    }

    return newStr;
}

Token Lexer::id(){
    std::string value;
    int currLineno = lineno;
    int currColumn = column;
    while (isalnum(current_char)){
        value += current_char;
        advance();
    }
    
    std::string keyword_value = toUpper(value);

    if (find(RESERVED_KEYWORDS.begin(), RESERVED_KEYWORDS.end(), keyword_value) != RESERVED_KEYWORDS.end()){
        TokenType type = StoR(keyword_value);
        return Token(type, keyword_value, currLineno, currColumn);
    } else {
        return Token(TokenType::ID, value, currLineno, currColumn);
    }
}

void Lexer::advance(){
    pos++;
    if (pos < text.length()){
        if (current_char == '\n'){
            lineno++;
            column = 1;
            current_char = text[pos];
        }
        else{
            current_char = text[pos];
            column++;
        }
            
    }
    else{
        current_char = '\0';
    }
}

char Lexer::peek(){
    if (pos + 1 < text.length()){
        return text[pos+1];
    } else {
        return '\0';
    }
}

Token Lexer::get_next_token(){
    while (current_char != '\0'){
        if (isspace(current_char) != 0){
            advance();
            continue;
        }

        if (current_char == '{'){
            while (current_char != '}'){
                advance();
            }
            advance();
            continue;
        }

        if (isalpha(current_char)){
            return id();
        }

        if (isdigit(current_char)){
            int currLineno = lineno;
            int currColumn = column;
            std::string value;
            do{
                value += current_char;
                advance();
            } while (isdigit(current_char));

            if (current_char == '.'){
                do{
                    value += current_char;
                    advance();
                } while (isdigit(current_char));
                return Token(TokenType::REAL_CONST, value, currLineno, currColumn); 
            }
            else{
                return Token(TokenType::INTEGER_CONST, value, currLineno, currColumn); 
            }                 
            
        }

        int currLineno = lineno;
        int currColumn = column;
        std::string val;
        switch (current_char){
            case '+': 
                advance();
                return Token(TokenType::ADD, "+", currLineno, currColumn); 
            case '-': 
                advance();
                return Token(TokenType::SUB, "-", currLineno, currColumn);
            case '*': 
                advance();
                return Token(TokenType::MUL, "*", currLineno, currColumn); 
            case '/': 
                advance();
                return Token(TokenType::DIV, "/", currLineno, currColumn); 
            case '(': 
                advance();
                return Token(TokenType::LPAREN, "(", currLineno, currColumn); 
            case ')':
                advance();
                return Token(TokenType::RPAREN, ")", currLineno, currColumn); 
            case '.':
                advance();
                return Token(TokenType::DOT, ".", currLineno, currColumn); 
            case ';':
                advance();
                return Token(TokenType::SEMI, ";", currLineno, currColumn); 
            case ':':
                if (peek() == '='){
                    advance();
                    advance();
                    return Token(TokenType::ASSIGN, ":=", currLineno, currColumn);
                } 
                advance();
                return Token(TokenType::COLON, ":", currLineno, currColumn); 
            case ',':
                advance();
                return Token(TokenType::COMMA, ",", currLineno, currColumn);
            case '>':
                if (peek() == '='){
                    advance(); 
                    advance();
                    return Token(TokenType::GTE, ">=", currLineno, currColumn);
                } 
                advance();
                return Token(TokenType::GREATER_THAN, ">", currLineno, currColumn);
            case '<':
                if (peek() == '='){
                    advance();
                    advance();
                    return Token(TokenType::LTE, "<=", currLineno, currColumn);
                }
                advance();
                return Token(TokenType::LESS_THAN, "<", currLineno, currColumn);
            case '=':
                advance();
                return Token(TokenType::EQUAL, "=", currLineno, currColumn);
            case '!':
                advance();
                if (current_char == '='){
                    advance(); 
                    return Token(TokenType::NOT_EQUAL, "!=", currLineno, currColumn);
                }
                error();
            case '\'':
                advance();
                return Token(TokenType::SINGLE_QUOTE, "'", currLineno, currColumn);
            case '"':
                advance();
                while (current_char != '"'){
                    val += current_char;
                    advance();
                }
                advance();
                return Token(TokenType::STRING_LITERAL, val, currLineno, currColumn);
            default:
                error(); 
        }
    }
        
    return Token(TokenType::END_OF_FILE, "END_OF_FILE", lineno, column);
}