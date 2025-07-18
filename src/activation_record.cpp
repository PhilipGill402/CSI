#include "activation_record.h"

ActivationRecord::ActivationRecord(std::string n, ARType t, int l): name(n), type(t), level(l){};

std::string ActivationRecord::toString(){
    std::string str = "";
    for (auto entry : members){
        if (entry.second != nullptr){
            str += entry.first + ": " + entry.second->toString() + "\n";
        }
    }

    return str;
}