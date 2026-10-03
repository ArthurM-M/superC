#pragma once
#include <string>
#include <vector>

#define ERROR_ORIGINS(X) \
    X(Lexer) \
    X(Parser)

#define AS_ENUM_ORI(name) name,

enum class ErrorOrigin {
    ERROR_ORIGINS(AS_ENUM_ORI)
};

#undef AS_ENUM_ORI

namespace Error {
    inline std::vector<std::string> errors;

    void report(ErrorOrigin origin, size_t line, size_t col, const std::string& message);
}