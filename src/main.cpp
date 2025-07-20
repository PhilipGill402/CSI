#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include "symbol.h"
#include "error.h"
#include <iostream>
#include <fstream>

int main(){
    //load text from file
    std::ifstream file("text.txt");
    std::string tmp;
    std::string text;
    
    while (getline(file, tmp)){
        text += tmp;
        text += "\n";
    }

    file.close();
    //tokenizing
    Lexer lexer = Lexer(text);
    Parser parser = Parser(lexer);
    AST* tree = parser.parse();
    SemanticAnalyzer semantics = SemanticAnalyzer();
    semantics.visit(tree);
    Interpreter interpreter = Interpreter(tree); 
    interpreter.interpret();
}