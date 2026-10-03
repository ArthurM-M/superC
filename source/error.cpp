#include <iostream>
#include <vector>
#include "error.h"

#define AS_IF_ORI(name) if(err == ErrorOrigin::name) return #name;
std::string origin_to_str(ErrorOrigin err) {
    ERROR_ORIGINS(AS_IF_ORI)
    return "Unknown";
}
#undef AS_IF_ORI

void Error::report(ErrorOrigin origin, size_t line, size_t col, const std::string& message) {
    std::string err = "[Line " + std::to_string(line) +":" + std::to_string(col) + "]" +
                      origin_to_str(origin) + " error: " + message;
                      
    errors.push_back(err);
}

