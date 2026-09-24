#pragma once
#include <string>
#include <fstream>

class Lexer {
    public:
        void tokenize(std::ifstream& file);
};