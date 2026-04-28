//
// Created by DELL on 4/28/2026.
//
#include "../../include/execution/lexer.h"
#include <cctype>
#include <stdexcept>
#include <algorithm>

Lexer::Lexer(const std::string& input) : input(input), position(0) {}

char Lexer::current_char() const {
    if (position >= input.size()) {
        return '\0';
    }
    return input[position];
}

char Lexer::peek_char() const {
    if (position + 1 >= input.size()) {
        return '\0';
    }
    return input[position + 1];
}

void Lexer::advance() {
    if (position < input.size()) {
        position++;
    }
}

void Lexer::skip_whitespace() {
    while (std::isspace(current_char())) {
        advance();
    }
}
std::string Lexer::to_upper(const std::string& word) const {
    std::string result = word;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return result;
}

bool Lexer::is_keyword(const std::string& word) const {
    static const std::unordered_set<std::string> keywords = {
        "SELECT",
        "FROM",
        "WHERE",
        "INSERT",
        "INTO",
        "VALUES",
        "CREATE",
        "TABLE",
        "DELETE",
        "UPDATE",
        "SET",
        "INT",
        "TEXT",
        "PRIMARY",
        "KEY"
    };

    return keywords.find(word) != keywords.end();
}

Token Lexer::read_word() {
    size_t start = position;
    std::string value;

    while (std::isalnum(current_char()) || current_char() == '_') {
        value += current_char();
        advance();
    }

    std::string upper_value = to_upper(value);

    if (is_keyword(upper_value)) {
        return Token(TokenType::KEYWORD, upper_value, static_cast<int>(start));
    }

    return Token(TokenType::IDENTIFIER, value, static_cast<int>(start));
}

Token Lexer::read_number() {
    size_t start = position;
    std::string value;

    while (std::isdigit(current_char())) {
        value += current_char();
        advance();
    }

    return Token(TokenType::NUMBER, value, static_cast<int>(start));
}

Token Lexer::read_string() {
    size_t start = position;
    std::string value;

    advance(); // skip opening quote

    while (current_char() != '\'' && current_char() != '\0') {
        value += current_char();
        advance();
    }

    if (current_char() == '\0') {
        return Token(TokenType::INVALID, "Unterminated string", static_cast<int>(start));
    }

    advance(); // skip closing quote

    return Token(TokenType::STRING, value, static_cast<int>(start));
}

Token Lexer::read_operator() {
    size_t start = position;
    std::string value;

    char current = current_char();
    char next = peek_char();

    if ((current == '<' || current == '>' || current == '!' || current == '=') && next == '=') {
        value += current;
        value += next;
        advance();
        advance();
        return Token(TokenType::OPERATOR, value, static_cast<int>(start));
    }

    if (current == '=' || current == '<' || current == '>') {
        value += current;
        advance();
        return Token(TokenType::OPERATOR, value, static_cast<int>(start));
    }

    value += current;
    advance();
    return Token(TokenType::INVALID, value, static_cast<int>(start));
}

Token Lexer::read_symbol() {
    size_t start = position;
    char c = current_char();

    advance();

    switch (c) {
        case ',':
            return Token(TokenType::COMMA, ",", static_cast<int>(start));
        case ';':
            return Token(TokenType::SEMICOLON, ";", static_cast<int>(start));
        case '(':
            return Token(TokenType::LEFT_PAREN, "(", static_cast<int>(start));
        case ')':
            return Token(TokenType::RIGHT_PAREN, ")", static_cast<int>(start));
        case '*':
            return Token(TokenType::STAR, "*", static_cast<int>(start));
        default:
            return Token(TokenType::INVALID, std::string(1, c), static_cast<int>(start));
    }
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (current_char() != '\0') {
        char c = current_char();

        if (std::isspace(static_cast<unsigned char>(c))) {
            skip_whitespace();
            continue;
        }

        if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
            tokens.push_back(read_word());
            continue;
        }

        if (std::isdigit(static_cast<unsigned char>(c))) {
            tokens.push_back(read_number());
            continue;
        }

        if (c == '\'') {
            tokens.push_back(read_string());
            continue;
        }

        if (c == '=' || c == '<' || c == '>' || c == '!') {
            tokens.push_back(read_operator());
            continue;
        }

        if (c == ',' || c == ';' || c == '(' || c == ')' || c == '*') {
            tokens.push_back(read_symbol());
            continue;
        }

        tokens.push_back(Token(TokenType::INVALID, std::string(1, c), static_cast<int>(position)));
        advance();
    }

    tokens.push_back(Token(TokenType::END_OF_FILE, "", static_cast<int>(position)));

    return tokens;
}