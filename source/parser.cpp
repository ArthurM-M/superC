#include "parser.h"

int precedence(TokenType type) {
    switch (type) {
        case TokenType::MULT:
        case TokenType::DIV:
            return 12;

        case TokenType::ADD:
        case TokenType::SUB:
            return 11;

        case TokenType::SHL:
        case TokenType::SHR:
            return 10;

        case TokenType::LESS:
        case TokenType::LEQ:
        case TokenType::GREATER:
        case TokenType::GEQ:
            return 9;

        case TokenType::EQ:
            return 8;

        case TokenType::BIT_AND:
            return 7;

        case TokenType::BIT_OR:
            return 6;

        case TokenType::AND:
            return 5;

        case TokenType::OR:
            return 4;

        case TokenType::ASSIGN:
            return 1;

        default:
            return 0;
    }
}

void parse_expression(size_t& cur, const std::vector<Token>& tokenized_text) {
    if(tokenized_text[cur].type == TokenType::IDENT || tokenized_text[cur].type == TokenType::NUMBER) {
        std::unique_ptr<Node> left = std::make_unique<Node>(nullptr, tokenized_text[cur], nullptr);
        cur++;
    } else {
        //Treat prefix case
    }

    while(precedence(tokenized_text[cur].type) != 0) {
        //Treat infix case
    }
}

void parse_statement(size_t& cur, const std::vector<Token>& tokenized_text) {
    //if tokenized_text[cur] == keyword -> parse_specific_keyword
    //else:
    parse_expression(cur, tokenized_text);
}


std::unique_ptr<Node> Parser::generate_ast(const std::vector<Token>& tokenized_text) {
    if(tokenized_text.empty()) return nullptr;

    std::unique_ptr<Node> root = std::make_unique<Node>(tokenized_text[0], nullptr, nullptr);
    size_t cur = 0;
    while(tokenized_text[cur].type != TokenType::END_OF_FILE) {
        parse_statement(cur, tokenized_text);
    }
    return root;
}