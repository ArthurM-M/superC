#pragma once
#include "lexer.h"
#include <memory>
#include <iostream>
#include <string>

struct Node {
    Token tk;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;

    Node(Token tk,
         std::unique_ptr<Node> left = nullptr,
         std::unique_ptr<Node> right = nullptr)
        : tk(std::move(tk)),
          left(std::move(left)),
          right(std::move(right)) {}
};

namespace Parser {
    std::unique_ptr<Node> generate_ast(const std::vector<Token>& tokenized_text);
}