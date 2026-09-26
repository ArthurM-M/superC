#include "lexer.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::ifstream file{"input.txt"};
    if(!file.is_open()) {
        std::cout << "Erro ao abrir arquivo\n";
        return 1;
    }
    print_tokens(Lexer::tokenize(file));

}