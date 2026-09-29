#pragma once
#include "lexer.h"
#include <memory>

struct Node {
    std::unique_ptr<Node> tk;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
};

namespace Parser {
    std::unique_ptr<Node> Parser::generate_ast(const std::vector<Token>& tokenized_text);
}