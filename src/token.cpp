#include "token.h"

using namespace std;

//returns the string value of the TokenType
string TtoS(TokenType type){
    switch (type){
        case TokenType::ADD: return "+";
        case TokenType::SUB: return "-";
        case TokenType::MUL: return "*";
        case TokenType::DIV: return "/";
        case TokenType::AND: return "AND";
        case TokenType::OR: return "OR";
        case TokenType::NOT: return "NOT";
        case TokenType::XOR: return "XOR";
        case TokenType::GREATER_THAN: return ">";
        case TokenType::EQUAL: return "==";
        case TokenType::NOT_EQUAL: return "!=";
        case TokenType::LESS_THAN: return "<";
        case TokenType::LTE: return "<=";
        case TokenType::GTE: return ">=";
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
        case TokenType::BOOLEAN : return "BOOLEAN";
        case TokenType::TRUE : return "TRUE";
        case TokenType::FALSE : return "FALSE";
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
        case TokenType::AND: return "AND";
        case TokenType::OR: return "OR";
        case TokenType::NOT: return "NOT";
        case TokenType::XOR: return "XOR";
        case TokenType::GREATER_THAN: return ">";
        case TokenType::EQUAL: return "==";
        case TokenType::NOT_EQUAL: return "!=";
        case TokenType::LESS_THAN: return "<";
        case TokenType::LTE: return "<=";
        case TokenType::GTE: return ">=";
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
        case TokenType::BOOLEAN: return "BOOLEAN";
        case TokenType::TRUE: return "TRUE";
        case TokenType::FALSE: return "FALSE";
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
    } else if (str == "BOOLEAN"){
        return TokenType::BOOLEAN;
    } else if (str == "AND"){
        return TokenType::AND;
    } else if (str == "NOT"){
        return TokenType::NOT;
    } else if (str == "OR"){
        return TokenType::OR;
    } else if (str == "XOR"){
        return TokenType::XOR;
    } else if (str == "TRUE"){
        return TokenType::TRUE;
    } else if (str == "FALSE"){
        return TokenType::FALSE;   
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