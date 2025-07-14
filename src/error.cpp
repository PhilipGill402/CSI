#include "error.h"

using namespace std;

string EtoS (ErrorCode error_code) {
    switch (error_code) {
        case ErrorCode::UNEXPECTED_TOKEN:
            return "Unexpected Token";
        case ErrorCode::ID_NOT_FOUND:
            return "Identifier Not Found";
        case ErrorCode::DUPLICATE_ID:
            return "Duplicate Identifier";
        case ErrorCode::INCORRECT_NUMBER_OF_ARGUMENTS:
            return "Incorrect Number of Arguments";
        default:
            return "Unknown Error Code";
    }
}
