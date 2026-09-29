#include "lexer.h"
#include "parser.h"
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
    auto tokens = Lexer::tokenize(file);
    print_tokens(tokens);
    Parser::generate_ast(tokens);
    return 0;
}