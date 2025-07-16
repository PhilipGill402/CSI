#pragma once
#include <string>

enum ErrorCode{
    UNEXPECTED_TOKEN,
    ID_NOT_FOUND,
    DUPLICATE_ID,
    INCORRECT_NUMBER_OF_ARGUMENTS,
    INCORRECT_TYPE,
    UNSUPPORTED_OPERATION,
    NONE
};

std::string EtoS(ErrorCode);