#include "lexer.h"
#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::ifstream file{"input.txt"};
    if(!file.is_open()) {
        std::cout << "Erro ao abrir arquivo\n";
    }
    Lexer lexer;
    lexer.tokenize(file);
}