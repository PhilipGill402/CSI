#include "helpers.h"

std::string toUpper(std::string str){
    std::string newStr; 
    for (char c : str){
        newStr += toupper(c);
    }

    return newStr;
}