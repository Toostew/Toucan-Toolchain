//
// Created by tooka on 06/10/2026.
//


#include "parser.h"
#include "exceptions.h"


token Parser::peek() {
    return tokens[pos];
}

bool Parser::check(const tokenType& type) {
    return Parser::peek().type == type;
}

void Parser::advance() {
    if (!(peek().type == tokenType::ENDOFFILE)) {
        pos++;
    } else {
        //error handling: EOL
        throw OutOfFile("Reached end of file before completing parsing");
    }

}

void Parser::expect(const tokenType& type) {
    if (Parser::peek().type == type) {
        advance();
    } else {
        //error handling here: not correct type
        std::string message = "Expected " + tokenTypeToString(type) +", got " + tokenTypeToString(Parser::peek().type);
        throw IncorrectTokenType("");
    }

}





