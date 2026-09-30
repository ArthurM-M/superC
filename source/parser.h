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

    virtual ~Node() = default;
};

struct UNode : Node {
    std::unique_ptr<Node> child;

    UNode(Token tk,
          std::unique_ptr<Node> child = nullptr)
        : Node(std::move(tk)),
          child(std::move(child)) {}
};

std::unique_ptr<Node> parse_expression(size_t& cur, const std::vector<Token>& tokens, int min_precedence = 0);

namespace Parser {
    std::unique_ptr<Node> generate_ast(const std::vector<Token>& tokenized_text);
}