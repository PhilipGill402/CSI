#pragma once
#include <string>

enum TokenType{
    ADD,
    SUB,
    MUL,
    DIV,
    END_OF_FILE,
    LPAREN,
    RPAREN,
    DOT,
    ID,
    ASSIGN,
    SEMI,
    COLON,
    COMMA,
    INTEGER_CONST,
    REAL_CONST,
    PROGRAM,
    INTEGER,
    REAL,
    INT_DIV,
    VAR,
    PROCEDURE,
    BEGIN,
    END,
    UNKNOWN
};

std::string TtoS(TokenType type);
std::string get_TokenType(TokenType type);

class Token{
public:
    TokenType type;
    std::string value;
    int lineno;
    int column;
    Token();
    Token(TokenType t, std::string val, int l, int c);
    std::string toString();
};

class Lexer{
public:
    std::string text;
    int pos;
    int lineno;
    int column;
    char current_char;
    Lexer(std::string txt);
    void advance();
    Token get_next_token();
};