#include <iostream>
#include <fstream>

using namespace std;

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

//returns the string value of the TokenType
string TtoS(TokenType type){
    switch (type){
        case TokenType::ADD: return "+";
        case TokenType::SUB: return "-";
        case TokenType::MUL: return "*";
        case TokenType::DIV: return "/";
        case TokenType::END_OF_FILE: return "EOF";
        case TokenType::LPAREN: return "(";
        case TokenType::RPAREN: return ")";
        case TokenType::DOT: return ".";
        case TokenType::ID: return "ID";
        case TokenType::ASSIGN: return ":=";
        case TokenType::SEMI: return ";";
        case TokenType::COLON: return ":";
        case TokenType::COMMA: return ",";
        case TokenType::INTEGER_CONST: return "INTEGER_CONST";
        case TokenType::PROGRAM: return "PROGRAM";
        case TokenType::INTEGER: return "INTEGER";
        case TokenType::REAL: return "REAL";
        case TokenType::INT_DIV: return "INT_DIV";
        case TokenType::VAR: return "VAR";
        case TokenType::PROCEDURE: return "PROCEDURE";
        case TokenType::BEGIN: return "BEGIN";
        case TokenType::END: return "END";
        default: return "UNKNOWN";
    }
}

string get_TokenType(TokenType type){
    switch (type){
        case TokenType::ADD: return "ADD";
        case TokenType::SUB: return "SUB";
        case TokenType::MUL: return "MUL";
        case TokenType::DIV: return "DIV";
        case TokenType::END_OF_FILE: return "END_OF_FILE";
        case TokenType::LPAREN: return "LPAREN";
        case TokenType::RPAREN: return "RPAREN";
        case TokenType::DOT: return "DOT";
        case TokenType::ID: return "ID";
        case TokenType::ASSIGN: return "ASSIGN";
        case TokenType::SEMI: return "SEMI";
        case TokenType::COLON: return "COLON";
        case TokenType::COMMA: return "COMMA";
        case TokenType::INTEGER_CONST: return "INTEGER_CONST";
        case TokenType::PROGRAM: return "PROGRAM";
        case TokenType::INTEGER: return "INTEGER";
        case TokenType::REAL: return "REAL";
        case TokenType::INT_DIV: return "INT_DIV";
        case TokenType::VAR: return "VAR";
        case TokenType::PROCEDURE: return "PROCEDURE";
        case TokenType::BEGIN: return "BEGIN";
        case TokenType::END: return "END";
        default: return "UNKNOWN";
    }
}

class Token{
public:
    TokenType type;
    string value;
    int lineno;
    int column;

    Token(): type(UNKNOWN), value("UNKNOWN"), lineno(0), column(0){};
    Token(TokenType t, string val, int l, int c): type(t), value(val), lineno(l), column(c){};

    string toString(){
        string s = "<TokenType." + get_TokenType(type) + ", " + value + ", " + to_string(lineno) + ":" + to_string(column) + ">";
        return s;
    }
};

class Lexer{
public:
    string text = "";
    int pos = 0;
    int lineno = 0;
    int column = 0;
    char current_char = text[0];

    Lexer(string txt): text(txt){};
    
    void advance(){
        if (pos++ < text.length()){
            if (current_char == '\n'){
                lineno++;
                column = 1;
                current_char = text[pos];
            }
            else{
                current_char = text[pos];
                column++;
            }
            
        }
    }

    Token get_next_token(){
        Token token = Token();
        while (token.value == "UNKNOWN"){
            if (isspace(current_char)){
                advance();
                continue;
            }

            if (current_char == '{'){
                while (current_char != '}'){
                    advance();
                }
                continue;
            }

            if (isdigit(current_char)){
                string value;
                do
                {
                    value += current_char;
                    advance();
                } while (isdigit(current_char));
                 
                token.type = TokenType::INTEGER_CONST;
                token.value = value;
                token.lineno = lineno;
                token.column = column;

                continue;
            }

            switch (current_char){
                case '+': 
                    advance();
                    token.type = TokenType::ADD;
                    token.value = "+";
                    token.lineno = lineno;
                    token.column = column;
                    break;
                case '-': 
                    advance();
                    token.type = TokenType::SUB;
                    token.value = "-";
                    token.lineno = lineno;
                    token.column = column;
                    break;
                case '*': 
                    advance();
                    token.type = TokenType::MUL;
                    token.value = "*";
                    token.lineno = lineno;
                    token.column = column;
                    break; 
                case '/': 
                    advance();
                    token.type = TokenType::DIV;
                    token.value = "/";
                    token.lineno = lineno;
                    token.column = column;
                    break;
                case '(': 
                    advance();
                    token.type = TokenType::LPAREN;
                    token.value = "(";
                    token.lineno = lineno;
                    token.column = column;
                    break;
                case ')':
                    advance();
                    token.type = TokenType::RPAREN;
                    token.value = ")";
                    token.lineno = lineno;
                    token.column = column;
                    break;
                case '.':
                    advance();
                    token.type = TokenType::DOT;
                    token.value = ".";
                    token.lineno = lineno;
                    token.column = column;
                    break;
                case ';':
                    advance();
                    token.type = TokenType::SEMI;
                    token.value = ";";
                    token.lineno = lineno;
                    token.column = column;
                    break;
                case ':':
                    advance();
                    token.type = TokenType::COLON;
                    token.value = ":";
                    token.lineno = lineno;
                    token.column = column;
                    break;
                case ',':
                    advance();
                    token.type = TokenType::COMMA;
                    token.value = ",";
                    token.lineno = lineno;
                    token.column = column;
                    break;
                default:
                    advance();
                    token.type = TokenType::END_OF_FILE;
                    token.value = "END_OF_FILE";
                    token.lineno = lineno;
                    token.column = column;
            }
        }
        
        return token;
    }
};

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
    Token token = lexer.get_next_token();
    while (token.type != TokenType::END_OF_FILE){
        cout << token.toString() << endl;
        token = lexer.get_next_token();
    }

}