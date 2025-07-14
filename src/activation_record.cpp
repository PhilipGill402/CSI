#include "activation_record.h"

using namespace std;

ActivationRecord::ActivationRecord(string n, ARType t, int l): name(n), type(t), level(l){};

string ActivationRecord::toString(){
    string str = "";
    for (auto entry : members){
        if (entry.second != nullptr){
            str += entry.first + ": " + to_string(entry.second->value) + "\n";
        }
    }

    return str;
}