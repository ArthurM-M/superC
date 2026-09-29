#include "parser.h"

int precedence(TokenType type) {
    switch (type) {
        case TokenType::MULT:
        case TokenType::DIV:    return 12;
        case TokenType::ADD:
        case TokenType::SUB:    return 11;
        case TokenType::SHL:
        case TokenType::SHR:    return 10;
        case TokenType::LESS:
        case TokenType::LEQ:
        case TokenType::GREATER:
        case TokenType::GEQ:    return 9;
        case TokenType::EQ:     return 8;
        case TokenType::BIT_AND:return 7;
        case TokenType::BIT_OR: return 6;
        case TokenType::AND:    return 5;
        case TokenType::OR:     return 4;
        case TokenType::ASSIGN: return 1;
        default:                return 0;
    }
}

std::unique_ptr<Node> parse_expression(size_t& cur, const std::vector<Token>& tokens, int min_precedence = 0) {
    std::unique_ptr<Node> left;
    if (cur >= tokens.size()) return nullptr;

    if(tokens[cur].type != TokenType::IDENT && tokens[cur].type != TokenType::NUMBER) {
        //treat prefix case
    } else {
        left = std::make_unique<Node>(tokens[cur++]);
    }

    while(cur < tokens.size()) {
        int op_precedence = precedence(tokens[cur].type);
        if(op_precedence <= min_precedence) break;

        Token op = tokens[cur++];

        auto right = parse_expression(cur, tokens, op_precedence);
        left = std::make_unique<Node>(op, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<Node> parse_statement(size_t& cur, const std::vector<Token>& tokens) {
    //if tokens[cur] == keyword -> parse_specific_keyword()
    //else:
    auto root = parse_expression(cur, tokens);
    return root;
}

void print_ast(const std::unique_ptr<Node>& node, int depth = 0) {
    if (!node)  return;

    for (int i = 0; i < depth; i++) {
        std::cout << "    ";
    }
    
    std::cout << "|-- " << node->tk.value << '\n';

    print_ast(node->left, depth + 1);
    print_ast(node->right, depth + 1);
}


std::unique_ptr<Node> Parser::generate_ast(const std::vector<Token>& tokens) {
    if(tokens.empty()) return nullptr;

    size_t cur = 0;
    auto root = parse_statement(cur, tokens);

    print_ast(root);
    return root;
}