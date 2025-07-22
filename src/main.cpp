#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include "symbol.h"
#include "error.h"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[]){
    if (argc < 2){
        std::cerr << "Error: expected a file\n";
        return -1;
    } 
    
    std::string file_path = argv[1]; 
    //load text from file
    std::ifstream file(file_path);
    if (!file){
        std::cerr << "Error: could not open file " << file_path << "\n";
        return -1;
    } 
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