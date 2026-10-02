#include "error.h"
#include "parser.h"

namespace {
    const std::vector<Token>* g_tokens = nullptr;
    size_t g_cursor = 0;

    bool is_at_end() {
        return !g_tokens || g_cursor >= g_tokens->size();
    }

    const Token& peek() {
        return (*g_tokens)[g_cursor];
    }

    Token advance() {
        if (!is_at_end()) return (*g_tokens)[g_cursor++];
        return (*g_tokens).back();
    }

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

    bool is_operand(TokenType type) {
        return type == TokenType::IDENT || type == TokenType::NUMBER;
    }

    std::unique_ptr<Node> parse_expression(int min_precedence = 0);

    std::unique_ptr<Node> parse_primary() {
        if (is_at_end()) return nullptr;

        if (is_operand(peek().type)) {
            return std::make_unique<Node>(advance());
        }

        if (peek().type == TokenType::LEFT_PAR) {
            advance();
            auto expr = parse_expression();

            if (is_at_end() || peek().type != TokenType::RIGHT_PAR) {
                Error::report(peek().position.y, peek().position.x, "Expected RIGHT_PAR");
            } else {
                advance();
            }
            return expr;
        }

        return nullptr; // Placeholder
    }

    std::unique_ptr<Node> parse_unary() {
        if (!is_at_end() && (peek().type == TokenType::ADD || peek().type == TokenType::SUB)) {
            Token op = advance();
            auto operand = parse_unary();

            if (!operand) {
                Error::report(peek().position.y, peek().position.x, "Expected OPERAND");
                return nullptr;
            }
            return std::make_unique<Node>(op, std::move(operand), nullptr);
        }

        return parse_primary();
    }

    std::unique_ptr<Node> parse_expression(int min_precedence) {
        auto left = parse_unary();
        if (!left) return nullptr;

        while (!is_at_end()) {
            int op_precedence = precedence(peek().type);
            if (op_precedence <= min_precedence) break;

            Token op = advance();

            auto right = parse_expression(op_precedence);
            left = std::make_unique<Node>(op, std::move(left), std::move(right));
        }

        return left;
    }

    std::unique_ptr<Node> parse_statement() {
        //if tokens[cur] == keyword -> parse_specific_keyword()
        //else:
        return parse_expression();
    }

    void print_ast(const std::unique_ptr<Node>& node, int depth = 0) {
        if (!node) return;

        for (int i = 0; i < depth; i++) {
            std::cout << "    ";
        }

        std::cout << "|-- " << node->tk.value << '\n';

        print_ast(node->left, depth + 1);
        print_ast(node->right, depth + 1);
    }

}

std::unique_ptr<Node> Parser::generate_ast(const std::vector<Token>& tokens) {
    if (tokens.empty()) return nullptr;
    g_tokens = &tokens;
    g_cursor = 0;

    auto root = parse_statement();
    print_ast(root);

    g_tokens = nullptr;
    g_cursor = 0;
    return root;
}