#include "lexer.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

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

#define AS_CASE_S_SYM(name, sym) case sym: return TokenType::name;

//Identify single character tokens
TokenType identify_token(char c) {
    switch (c) {
        SINGLE_SYMBOL_TOKENS(AS_CASE_S_SYM)
        default:  return TokenType::UNKNOWN;
    }
}

#undef AS_CASE_S_SYM


#define AS_CASE_SYM(name, sym) case TokenType::name: return #name;
#define AS_CASE_SPE(name) case TokenType::name: return #name;

std::string token_to_str(TokenType type) {
    switch (type) {
        SINGLE_SYMBOL_TOKENS(AS_CASE_SYM)
        MULTI_SYMBOL_TOKENS(AS_CASE_SYM)
        SPECIAL_TOKENS(AS_CASE_SPE)
    }
    return "UNKNOWN";
}

#undef AS_CASE_SYM
#undef AS_CASE_SPE

void print_tokens(std::vector<Token> tokenized_text) {
    for(auto x : tokenized_text) {
        std::cout << token_to_str(x.type) << "(" << x.value
                  << ") pos x:" << x.position.x << " pos y:" << x.position.y << "\n";
    }
}

std::vector<Token> Lexer::tokenize(std::ifstream& file) {
    std::vector<Token> tokenized_text;

    constexpr std::string_view DELIMITERS = "+-*/();,=";
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
            tokenized_text.push_back({identify_token(c), std::string(1, c), {pos_x, pos_y}});  //Tokenize char

        } else {
            if(buf.empty()) {
                pos_x_start = pos_x;
            }
            buf.push_back(c);
        }
        pos_x++;
    }
    if (!buf.empty()) { //Tokenize the last string (chars processed in loop)
        tokenized_text.push_back({identify_token(buf), buf, {pos_x_start, pos_y}});
    }

    return tokenized_text;
}