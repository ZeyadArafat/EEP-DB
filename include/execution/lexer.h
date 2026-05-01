//
// Created by DELL on 4/28/2026.
//

#ifndef EEPDB_LEXER_H
#define EEPDB_LEXER_H


#include "token.h"
#include <string>
#include <vector>
#include <unordered_set>

class Lexer {
public:
    explicit Lexer(const std::string& input);

    std::vector<Token> tokenize();

private:
    std::string input;
    size_t position; //  unsigned integer type used for sizes and indexing

    char current_char() const;
    char peek_char() const;
    void advance();

    void skip_whitespace();

    Token read_word();
    Token read_number();
    Token read_string();
    Token read_operator();
    Token read_symbol();

    bool is_keyword(const std::string& word) const;
    std::string to_upper(const std::string& word) const;
};

#endif //EEPDB_LEXER_H
