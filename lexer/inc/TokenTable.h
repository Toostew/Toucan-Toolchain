//
// Created by tooka on 27/09/2026.
//

#ifndef TOKENTABLE_H
#define TOKENTABLE_H

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>




enum class tokenType {
    //keywords/reserved general words
    IF, ELSE, WHILE, FOR, RETURN, VOID,
    //reserved type words
    INT, BOOL,
    //Operators
    PLUS, MINUS, MULTIPLY, DIVIDE,
    //logical Comparators
    LG_AND, LG_OR, LG_NOT, LG_EQUAL, LG_N_EQUAL, LG_GREATER, LG_LESS, LG_GREATER_EQUAL, LG_LESS_EQUAL,
    //Assignment
    ASSIGNMENT,
    //Identifier and literals
    IDENTIFIER, INT_LITERAL, BOOL_TRUE, BOOL_FALSE,
    //reserved punctuation
    LPAREN, RPAREN, LBRACE, RBRACE, SEMICOLON, COMMA,

    //ERR
    ERR
};

struct token {
    std::string text;
    tokenType type;
};


class TokenTable {
    public:
        TokenTable();
        std::vector<std::pair<std::string, tokenType>> getTokenTableEntries(); //returns all pairs in the tokenTable
        tokenType getToken(std::string key);


    private:
        void addToken(std::string key, tokenType token);
        std::unordered_map<std::string, tokenType> tokenTable; //hashmap of every single token

};

#endif //NTR_TOOLCHAIN_LEXER_H