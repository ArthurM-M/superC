#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <cstddef>

// X(EnumName, Symbol)
#define SINGLE_SYMBOL_TOKENS(X) \
    X(ADD,          '+')  \
    X(SUB,          '-')  \
    X(MULT,         '*')  \
    X(DIV,          '/')  \
    X(ASSIGN,       '=')  \
    X(LEFT_PAR,     '(')  \
    X(RIGHT_PAR,    ')')  \
    X(COMMA,        ',')  \
    X(SEMICOLON,    ';')

// X(EnumName, Symbol)
#define MULTI_SYMBOL_TOKENS(X) \
    X(EQ,   "==")

//X(EnumName)
#define SPECIAL_TOKENS(X) \
    X(NUMBER)  \
    X(IDENT)   \
    X(UNKNOWN)

#define AS_ENUM_SYM(name, sym) name,
#define AS_ENUM_SPE(name) name,

enum class TokenType {
    SINGLE_SYMBOL_TOKENS(AS_ENUM_SYM)
    MULTI_SYMBOL_TOKENS(AS_ENUM_SYM)
    SPECIAL_TOKENS(AS_ENUM_SPE)
};

#undef AS_ENUM_SYM
#undef AS_ENUM_SPE

struct TokenPosition {
    std::size_t x, y;
};

struct Token {
    TokenType type;
    std::string value;
    TokenPosition position;
};

void print_tokens(const std::vector<Token> tokenized_text);

namespace Lexer {
    std::vector<Token> tokenize(std::ifstream& file);
};