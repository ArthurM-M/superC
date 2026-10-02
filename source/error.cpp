#include <iostream>
#include "error.h"

void Error::report(int line, int col, const std::string& message) {
    std::cerr << "[Line " << line << ":" << col << "] Error: " 
                << message << '\n';
}