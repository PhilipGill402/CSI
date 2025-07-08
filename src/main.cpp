#include "lexer.h"
#include "parser.h"
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
    parser.expr();   
 
}