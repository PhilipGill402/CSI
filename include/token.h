#pragma once
#include <string>
#include <stdexcept>
#include "token.h"

enum TokenType{
    ADD,
    SUB,
    MUL,
    DIV,
    SINGLE_QUOTE,
    DOUBLE_QUOTE,
    AND,
    OR,
    NOT,
    XOR,
    EQUAL,
    NOT_EQUAL,
    LESS_THAN,
    GREATER_THAN,
    LTE,
    GTE,
    END_OF_FILE,
    LPAREN,
    RPAREN,
    LBRACKET,
    RBRACKET,
    DOT,
    ID,
    ASSIGN,
    SEMI,
    COLON,
    COMMA,
    INTEGER_CONST,
    REAL_CONST,
    STRING_LITERAL,
    PROGRAM,
    INTEGER,
    REAL,
    BOOLEAN,
    CHAR,
    STRING,
    ARRAY,
    OF,
    TRUE,
    FALSE,
    INT_DIV,
    VAR,
    PROCEDURE,
    BEGIN,
    END,
    IF,
    THEN,
    ELSE,
    WHILE,
    DO,
    UNKNOWN
};

std::string TtoS(TokenType);
std::string get_TokenType(TokenType);
TokenType StoR(std::string);

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