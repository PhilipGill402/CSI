#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

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

std::string TtoS(TokenType);
std::string get_TokenType(TokenType);

class Token{
public:
    TokenType type;
    std::string value;
    int lineno;
    int column;
    Token();
    Token(TokenType, std::string, int, int);
    std::string toString();
};

class Lexer{
public:
    const std::vector<std::string> RESERVED_KEYWORDS = {"PROGRAM", "INTEGER", "REAL", "DIV", "VAR", "PROCEDURE", "BEGIN", "END"};
    std::string text;
    int pos;
    int lineno;
    int column;
    char current_char;
    Lexer(std::string txt);
    void advance();
    Token get_next_token();
    Token id();
};