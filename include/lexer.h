#pragma once
#include "token.h"
#include "error.h"
#include "helpers.h"
#include "iostream"
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

class Lexer{
public:
    const std::vector<std::string> RESERVED_KEYWORDS = {"PROGRAM", "INTEGER", "REAL", "BOOLEAN", "CHAR", "STRING", "ARRAY", "OF", "TRUE", "FALSE", "AND", "OR", "NOT", "XOR", "DIV", "VAR", "PROCEDURE", "BEGIN", "END", "IF", "THEN", "ELSE", "WHILE", "DO", "FOR", "TO", "DOWNTO", "REPEAT", "UNTIL", "FUNCTION"};
    std::string text;
    int pos;
    int lineno;
    int column;
    char current_char;
    Lexer(std::string txt);
    void error();
    void advance();
    char peek();
    Token get_next_token();
    Token id();
};