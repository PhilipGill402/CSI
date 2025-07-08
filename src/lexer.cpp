#include "lexer.h"
#include <iostream>
#include <fstream>

using namespace std;

//returns the string value of the TokenType
string TtoS(TokenType type){
    switch (type){
        case TokenType::ADD: return "+";
        case TokenType::SUB: return "-";
        case TokenType::MUL: return "*";
        case TokenType::DIV: return "/";
        case TokenType::END_OF_FILE: return "EOF";
        case TokenType::LPAREN: return "(";
        case TokenType::RPAREN: return ")";
        case TokenType::DOT: return ".";
        case TokenType::ID: return "ID";
        case TokenType::ASSIGN: return ":=";
        case TokenType::SEMI: return ";";
        case TokenType::COLON: return ":";
        case TokenType::COMMA: return ",";
        case TokenType::INTEGER_CONST: return "INTEGER_CONST";
        case TokenType::REAL_CONST: return "REAL_CONST";
        case TokenType::PROGRAM: return "PROGRAM";
        case TokenType::INTEGER: return "INTEGER";
        case TokenType::REAL: return "REAL";
        case TokenType::INT_DIV: return "INT_DIV";
        case TokenType::VAR: return "VAR";
        case TokenType::PROCEDURE: return "PROCEDURE";
        case TokenType::BEGIN: return "BEGIN";
        case TokenType::END: return "END";
        default: return "UNKNOWN";
    }
}

string get_TokenType(TokenType type){
    switch (type){
        case TokenType::ADD: return "ADD";
        case TokenType::SUB: return "SUB";
        case TokenType::MUL: return "MUL";
        case TokenType::DIV: return "DIV";
        case TokenType::END_OF_FILE: return "END_OF_FILE";
        case TokenType::LPAREN: return "LPAREN";
        case TokenType::RPAREN: return "RPAREN";
        case TokenType::DOT: return "DOT";
        case TokenType::ID: return "ID";
        case TokenType::ASSIGN: return "ASSIGN";
        case TokenType::SEMI: return "SEMI";
        case TokenType::COLON: return "COLON";
        case TokenType::COMMA: return "COMMA";
        case TokenType::INTEGER_CONST: return "INTEGER_CONST";
        case TokenType::REAL_CONST: return "REAL_CONST";
        case TokenType::PROGRAM: return "PROGRAM";
        case TokenType::INTEGER: return "INTEGER";
        case TokenType::REAL: return "REAL";
        case TokenType::INT_DIV: return "INT_DIV";
        case TokenType::VAR: return "VAR";
        case TokenType::PROCEDURE: return "PROCEDURE";
        case TokenType::BEGIN: return "BEGIN";
        case TokenType::END: return "END";
        default: return "UNKNOWN";
    }
}

TokenType StoR(string str){
    if (str == "PROGRAM"){
        return TokenType::PROGRAM;
    } else if (str == "INTEGER"){
        return TokenType::INTEGER;
    } else if (str == "REAL"){
        return TokenType::REAL;
    } else if (str == "DIV"){
        return TokenType::DIV;
    } else if (str == "VAR"){
        return TokenType::VAR;
    } else if (str == "PROCEDURE"){
        return TokenType::PROCEDURE;
    } else if (str == "BEGIN"){
        return TokenType::BEGIN;
    } else if (str == "END"){
        return TokenType::END;
    } else {
        throw invalid_argument("Invalid string passed to 'StoR'");
    }
}

Token::Token(): type(UNKNOWN), value("UNKNOWN"), lineno(0), column(0){};
Token::Token(TokenType t, string val, int l, int c): type(t), value(val), lineno(l), column(c){};

string Token::toString(){
    string s = "<TokenType." + get_TokenType(type) + ", " + value + ", " + to_string(lineno) + ":" + to_string(column) + ">";
    return s;
}

Lexer::Lexer(string txt): text(txt), pos(0), lineno(1), column(1), current_char(text.empty() ? '\0' : txt[0]){};

string toUpper(string str){
    string newStr; 
    for (char c : str){
        newStr += toupper(c);
    }

    return newStr;
}

Token Lexer::id(){
    string value;
    int currLineno = lineno;
    int currColumn = column;
    while (isalpha(current_char)){
        value += current_char;
        advance();
    }
    value = toUpper(value);

    if (find(RESERVED_KEYWORDS.begin(), RESERVED_KEYWORDS.end(), value) != RESERVED_KEYWORDS.end()){
        TokenType type = StoR(value);
        return Token(type, value, currLineno, currColumn);
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
            continue;
        }

        if (isalpha(current_char)){
            return id();
        }

        if (isdigit(current_char)){
            int currLineno = lineno;
            int currColumn = column;
            string value;
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
                advance();
                return Token(TokenType::COLON, ":", currLineno, currColumn); 
            case ',':
                advance();
                return Token(TokenType::COMMA, ",", currLineno, currColumn); 
            default:
                advance();
                continue; 
        }
    }
        
    return Token(TokenType::END_OF_FILE, "END_OF_FILE", lineno, column);
}