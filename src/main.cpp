#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
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
    Interpreter interpreter = Interpreter(tree); 
    interpreter.interpret();
    for (auto entry : interpreter.global_variables){
        cout << entry.first << ": " << entry.second->value << '\n';
    }
    


    /*
    Token token = parser.current_token;
    while (token.type != TokenType::END_OF_FILE){
        cout << token.toString() << '\n';
        token = parser.get_next_token();
    }
    */
}