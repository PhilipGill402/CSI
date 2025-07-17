#pragma once
#include "token.h"
#include "error.h"
#include "iostream"
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

class Lexer{
public:
    const std::vector<std::string> RESERVED_KEYWORDS = {"PROGRAM", "INTEGER", "REAL", "BOOLEAN", "CHAR", "STRING", "ARRAY", "OF", "TRUE", "FALSE", "AND", "OR", "NOT", "XOR", "DIV", "VAR", "PROCEDURE", "BEGIN", "END", "IF", "THEN", "ELSE"};
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