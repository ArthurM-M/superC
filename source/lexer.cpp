#include "lexer.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

enum class TokenType {
    NUMBER,
    IDENT,
    ADD,
    SUB,
    MULT,
    DIV,
    LEFT_PAR,
    RIGHT_PAR,
    COMMA,
    SEMICOLON,
    UNKNOWN
};

struct TokenPosition {
    std::size_t x, y;
};

struct Token {
    TokenType type;
    std::string value;
    TokenPosition position;
};

bool is_number(const std::string& v) {  //Requires non-empty string
    if(v[0] == '.' || v[v.size() - 1] == '.') {
        return false;
    }
    bool one_dot = false;
    for(size_t t = 0; t < v.size(); t++) {
        if((v[t] < '0' || v[t] > '9') && v[t] != '.') {
            return false;
        }
        if(one_dot && v[t] == '.') {
            return false;
        }
        if(v[t] == '.') {
            one_dot = true;
        }
    }
    return true;
}

TokenType identify_token(const std::string& buf) {
    if(is_number(buf)) return TokenType::NUMBER;
    return TokenType::IDENT;
}

TokenType identify_token(char c) {
    switch (c) {
        case '+': return TokenType::ADD;
        case '-': return TokenType::SUB;
        case '*': return TokenType::MULT;
        case '/': return TokenType::DIV;
        case '(': return TokenType::LEFT_PAR;
        case ')': return TokenType::RIGHT_PAR;
        case ',': return TokenType::COMMA;
        case ';': return TokenType::SEMICOLON;
        default:  return TokenType::UNKNOWN;
    }
}

std::string token_type_name(TokenType type) {
    switch (type) {
        case TokenType::IDENT:      return "IDENT";
        case TokenType::ADD:        return "ADD";
        case TokenType::SUB:        return "SUB";
        case TokenType::MULT:       return "MULT";
        case TokenType::DIV:        return "DIV";
        case TokenType::NUMBER:     return "NUMBER";
        case TokenType::LEFT_PAR:        return "LEFT_PAR";
        case TokenType::RIGHT_PAR:       return "RIGHT_PAR";
        case TokenType::COMMA:       return "COMMA";
        case TokenType::SEMICOLON:       return "SEMICOLON";
        case TokenType::UNKNOWN:    return "UNKNOWN";
    }
    return "UNKNOWN";
}

void Lexer::tokenize(std::ifstream& file) {
    std::vector<Token> tokenized_text;

    constexpr std::string_view DELIMITERS = "+-*/();,";
    constexpr std::string_view WHITESPACES = " \n\t";

    std::size_t pos_x = 1, pos_x_start = 1, pos_y = 1;
    char c; std::string buf;
    while(file.get(c)) {
        if(WHITESPACES.find(c) != std::string_view::npos) {
            if(!buf.empty()) {
                tokenized_text.push_back({identify_token(buf), buf, {pos_x_start, pos_y}});
                buf.clear();
            }
            if(c == '\n') { //Next line positions
                pos_x = 0;
                pos_y++;
            }

        } else if (DELIMITERS.find(c) != std::string_view::npos){
            if(!buf.empty()) {
                tokenized_text.push_back({identify_token(buf), buf, {pos_x_start, pos_y}});
                buf.clear();
            }
            tokenized_text.push_back({identify_token(c), "", {pos_x, pos_y}});  //Tokenize char

        } else {
            if(buf.empty()) {
                pos_x_start = pos_x;
            }
            buf.push_back(c);
        }
        pos_x++;
    }

    if (!buf.empty()) {
        if(DELIMITERS.find(c) != std::string_view::npos) {
            tokenized_text.push_back({identify_token(buf[0]), "", {pos_x_start, pos_y}});
        } else {
            tokenized_text.push_back({identify_token(buf), buf, {pos_x_start, pos_y}});
        }
    }

    for(auto x : tokenized_text) {
        std::cout << token_type_name(x.type) << "(" << x.value
                  << ") pos x:" << x.position.x << " pos y:" << x.position.y << "\n";
    }

}