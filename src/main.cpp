#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include "symbol.h"
#include "error.h"
#include <iostream>
#include <fstream>

using namespace std;

int main(){
    //load text from file
    ifstream file("text.txt");
    string tmp;
    string text;
    
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
    
    for (auto entry : interpreter.global_variables){
        if (entry.second != nullptr){
            cout << entry.first << ": " << entry.second->value << '\n';
        } 
    }


    /*
    Token token = parser.current_token;
    while (token.type != TokenType::END_OF_FILE){
        cout << token.toString() << '\n';
        token = parser.get_next_token();
    }
    */
}