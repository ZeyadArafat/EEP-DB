//
// Created by DELL on 4/28/2026.
//

#ifndef EEPDB_TOKEN_H
#define EEPDB_TOKEN_H


using namespace std;

#include <iostream>;
#include <string>

// fixed set of options so we use enum
enum class TokenType {
    KEYWORD,
    IDENTIFIER,
    NUMBER,
    STRING,

    COMMA,          // ,
    SEMICOLON,      // ;
    LEFT_PAREN,     // (
    RIGHT_PAREN,    // )
    STAR,           // *

    OPERATOR,       // =, <, >, <=, >=, !=

    END_OF_FILE,
    INVALID
};

struct Token {
    TokenType type;
    string value;
    int position;

    Token(TokenType t, const string& v, int pos) : type(t), value(v), position(pos) {}


};

#endif