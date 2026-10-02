#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <cstddef>

// X(EnumName, Symbol)
#define SINGLE_SYMBOL_TOKENS(X) \
    X(ADD,          '+') \
    X(SUB,          '-') \
    X(MULT,         '*') \
    X(DIV,          '/') \
    X(BIT_AND,       '&') \
    X(BIT_OR,        '|') \
    X(BIT_NOT,      '~') \
    X(NOT,           '!') \
    X(ASSIGN,       '=') \
    X(LESS,          '<') \
    X(GREATER,       '>') \
    X(LEFT_PAR,     '(') \
    X(RIGHT_PAR,    ')') \
    X(COMMA,        ',') \
    X(SEMICOLON,    ';')

// X(EnumName, Symbol)
#define MULTI_SYMBOL_TOKENS(X) \
    X(EQ,   "==") \
    X(NEQ,  "!=") \
    X(GEQ,  ">=") \
    X(LEQ,  "<=") \
    X(AND,  "&&") \
    X(OR,   "||") \
    X(SHL,  "<<") \
    X(SHR,  ">>")

//X(EnumName)
#define SPECIAL_TOKENS(X) \
    X(NUMBER)  \
    X(IDENT)   \
    X(UNKNOWN) \
    X(END_OF_FILE)

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
    std::size_t col, line;
};

struct Token {
    TokenType type;
    std::string value;
    TokenPosition position;
};

void print_tokens(const std::vector<Token>& tokenized_text);

namespace Lexer {
    std::vector<Token> tokenize(std::ifstream& file);
};