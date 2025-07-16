#pragma once
#include <stack>
#include <string>
#include <unordered_map>
#include "ast.h"

enum ARType{
    AR_PROGRAM,
    AR_PROCEDURE
};


class ActivationRecord{
public:
    std::string name;
    ARType type;
    int level;
    std::unordered_map<std::string, Value*> members = {};
    
    ActivationRecord(std::string, ARType, int);
    std::string toString();
};

class CallStack{
public:
    std::stack<ActivationRecord> records;
};