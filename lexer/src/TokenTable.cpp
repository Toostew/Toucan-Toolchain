//
// Created by tooka on 27/09/2026.
//

#include "TokenTable.h"

int main() {


    return 0;
}


//populate the tokenTable at class init
TokenTable::TokenTable() {
    //reserved keywords
    addToken("if",tokenType::IF);
    addToken("else",tokenType::ELSE);
    addToken("while",tokenType::WHILE);
    addToken("for",tokenType::FOR);
    addToken("return",tokenType::RETURN);

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

