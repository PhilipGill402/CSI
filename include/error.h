#pragma once
#include <string>

enum ErrorCode{
    UNEXPECTED_TOKEN,
    ID_NOT_FOUND,
    DUPLICATE_ID,
    INCORRECT_NUMBER_OF_ARGUMENTS,
    NONE
};

std::string EtoS(ErrorCode);