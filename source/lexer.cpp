#include "lexer.h"
#include "error.h"
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

bool is_ident(const std::string& v) {   //Requires non-empty string
    if(v[0] >= '0' && v[0] <= '9') {
        return false;
    }
    for(size_t t = 0; t < v.size(); t++) {  
        if((v[t] < 'a' || v[t] > 'z') && (v[t] < 'A' || v[t] > 'Z') && (v[t] < '0' || v[t] > '9') && v[t] != '_') {
            return false;
        }
    }
    return true;
}

#define AS_IF_SYM(name, sym) if (buf == sym) return TokenType::name;

//Identify multi character tokens
TokenType identify_token(const std::string& buf) {
    MULTI_SYMBOL_TOKENS(AS_IF_SYM)
    if(is_number(buf)) return TokenType::NUMBER;
    if(is_ident(buf)) return TokenType::IDENT;
    return TokenType::UNKNOWN;
}

#undef AS_IF_SYM

#define AS_CASE_S_SYM(name, sym) case sym: return TokenType::name;

//Identify single character tokens
TokenType identify_token(char c) { 
    switch (c) {
        SINGLE_SYMBOL_TOKENS(AS_CASE_S_SYM)
        default:
            return TokenType::UNKNOWN;
    }
}

Token emit_token(std::string &buf, size_t col, size_t line) {
    auto type = identify_token(buf);

    if(type == TokenType::UNKNOWN) {
        Error::report(line, col, "UNKNOWN token: '" + buf + "'.");
    }   
    return {type, buf, {col, line}};
}

Token emit_token(char c, size_t col, size_t line) {
    auto type = identify_token(c);

    if(type == TokenType::UNKNOWN) {
        Error::report(line, col, std::string("UNKNOWN token: '") + c + "'.");
    }   
    return {type, std::string(1, c), {col, line}};
}

#undef AS_CASE_S_SYM

#define AS_CASE_SYM(name, sym) case sym: return true;

bool is_s_symbol(char c) {
    switch (c) {
        SINGLE_SYMBOL_TOKENS(AS_CASE_SYM)
    }
    return false;
}

#undef AS_CASE_SYM

#define AS_IF_SYM(name, sym) if (s == sym) return true;

bool is_m_symbol(const std::string& s) {
    MULTI_SYMBOL_TOKENS(AS_IF_SYM)
    return false;
}

#undef AS_IF_SYM

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

void print_tokens(const std::vector<Token>& tokenized_text) {
    for(auto x : tokenized_text) {
        std::cout << token_to_str(x.type) << "(" << x.value
                  << ") pos x:" << x.position.col << " pos y:" << x.position.line << "\n";
    }
}


std::vector<Token> Lexer::tokenize(std::ifstream& file) {
    std::vector<Token> tokenized_text;

    std::size_t cur_col = 1, col_start = 1, line = 1;
    char c; std::string buf;
    while(file.get(c)) {
        if(std::isspace(c)) {
            if(!buf.empty()) {  //Push buffer
                tokenized_text.push_back(emit_token(buf, col_start, line));
                buf.clear();
            }
            if(c == '\n') { //Next line positions
                cur_col = 0;
                line++;
            }

        } else if (is_s_symbol(c)){
            if(!buf.empty()) {  //Push buffer
                tokenized_text.push_back(emit_token(buf, col_start, line));
                buf.clear();
            }

            //Tokenize char
            int next = file.peek();
            std::string sym;
            auto aux = std::string(1, c);
            while(next != EOF) {
                aux += static_cast<char>(next);
                if(is_m_symbol(aux)) {
                    sym = aux;
                    file.get(c);
                    next = file.peek();
                    cur_col++;
                } else {
                    break;
                }
            }
            
            if(sym.empty()) {
                tokenized_text.push_back(emit_token(buf, col_start, line)); 
            } else {
                tokenized_text.push_back(emit_token(buf, col_start, line)); 
            }
            
        } else {
            if(buf.empty()) {
                col_start = cur_col;    //col start -> beginning of a token; cur col -> current position
            }
            buf.push_back(c);
        }
        cur_col++;
    }
    if (!buf.empty()) { //Tokenize the last string (chars processed in loop)
        tokenized_text.push_back({identify_token(buf), buf, {col_start, line}});
    }
    tokenized_text.push_back({TokenType::END_OF_FILE, "EOF", {cur_col, line}});
    return tokenized_text;
}