//
// Created by tooka on 27/09/2026.
//

#include "TokenTable.h"




//populate the tokenTable at class init
TokenTable::TokenTable() {
    //reserved keywords
    addToken("if",tokenType::IF);
    addToken("else",tokenType::ELSE);
    addToken("while",tokenType::WHILE);
    addToken("for",tokenType::FOR);
    addToken("return",tokenType::RETURN);

    //reserved types
    addToken("int",tokenType::INT);
    addToken("bool",tokenType::BOOL);

    //Operators
    addToken("+", tokenType::PLUS);
    addToken("-", tokenType::MINUS);
    addToken("*", tokenType::MULTIPLY);
    addToken("/", tokenType::DIVIDE);

    //Logical Op
    addToken("&&", tokenType::LG_AND);
    addToken("||", tokenType::LG_OR);
    addToken("!", tokenType::LG_NOT);
    addToken("==", tokenType::LG_EQUAL);
    addToken("!=", tokenType::LG_N_EQUAL);
    addToken(">", tokenType::LG_GREATER);
    addToken("<", tokenType::LG_LESS);
    addToken(">=", tokenType::LG_GREATER_EQUAL);
    addToken("<=", tokenType::LG_LESS_EQUAL);

    //Assignment
    addToken("=", tokenType::ASSIGNMENT);

    //Identifier and Literal
    //addToken("", tokenType::); //we havent killed tokens
    //addToken("", tokenType::);
    addToken("true", tokenType::BOOL_TRUE);
    addToken("false", tokenType::BOOL_FALSE);

    //reserved punctuation
    addToken("(", tokenType::LPAREN);
    addToken(")", tokenType::RPAREN);
    addToken("{", tokenType::LBRACE);
    addToken("}", tokenType::RBRACE);
    addToken(";", tokenType::SEMICOLON);
    addToken(",", tokenType::COMMA);

}

std::vector<std::pair<std::string, tokenType> > TokenTable::getTokenTableEntries() {
    std::vector<std::pair<std::string, tokenType> > tokenTableEntries;
    for (const std::pair<std::string, tokenType>& token : TokenTable::tokenTable) {
        tokenTableEntries.push_back(token);
    }

    return tokenTableEntries;
}


tokenType TokenTable::getToken(std::string key) {
    auto iter = TokenTable::tokenTable.find(key);
    if (iter != TokenTable::tokenTable.end()) {
        return iter->second;
    } else {
        std::cout << "Key not found: " << key << std::endl;
        return tokenType::ERR;
    }
}


void TokenTable::addToken(std::string key, tokenType tokentype) {
    tokenTable[key] = tokentype;
}


