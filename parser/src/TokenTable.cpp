//
// Created by tooka on 27/09/2026.
//

#include "TokenTable.h"




//populate the tokenTable at class init
TokenTable::TokenTable() {
    //reserved keywords
    addToken("IF",tokenType::IF);
    addToken("ELSE",tokenType::ELSE);
    addToken("WHILE",tokenType::WHILE);
    addToken("FOR",tokenType::FOR);
    addToken("RETURN",tokenType::RETURN);

    //reserved types
    addToken("INT",tokenType::INT);
    addToken("BOOL",tokenType::BOOL);
    addToken("VOID", tokenType::VOID);

    //Operators
    addToken("PLUS", tokenType::PLUS);
    addToken("MINUS", tokenType::MINUS);
    addToken("MULTIPLY", tokenType::MULTIPLY);
    addToken("DIVIDE", tokenType::DIVIDE);

    //Logical Op
    addToken("LG_AND", tokenType::LG_AND);
    addToken("LG_OR", tokenType::LG_OR);
    addToken("LG_NOT", tokenType::LG_NOT);
    addToken("LG_EQUAL", tokenType::LG_EQUAL);
    addToken("LG_N_EQUAL", tokenType::LG_N_EQUAL);
    addToken("LG_GREATER", tokenType::LG_GREATER);
    addToken("LG_LESS", tokenType::LG_LESS);
    addToken("LG_GREATER_EQUAL", tokenType::LG_GREATER_EQUAL);
    addToken("LG_LESS_EQUAL", tokenType::LG_LESS_EQUAL);

    //Assignment
    addToken("ASSIGNMENT", tokenType::ASSIGNMENT);

    //Identifier and Literal
    addToken("INT_LITERAL", tokenType::INT_LITERAL);
    addToken("BOOL_TRUE", tokenType::BOOL_TRUE);
    addToken("BOOL_FALSE", tokenType::BOOL_FALSE);

    //reserved punctuation
    addToken("LPAREN", tokenType::LPAREN);
    addToken("RPAREN", tokenType::RPAREN);
    addToken("LBRACE", tokenType::LBRACE);
    addToken("RBRACE", tokenType::RBRACE);
    addToken("SEMICOLON", tokenType::SEMICOLON);
    addToken("COMMA", tokenType::COMMA);

    addToken("IDENTIFIER", tokenType::IDENTIFIER);
    addToken("ENDOFFILE", tokenType::ENDOFFILE);
    addToken("ERR", tokenType::ERR);

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
        return tokenType::ERR;
    }
}


void TokenTable::addToken(std::string key, tokenType tokentype) {
    tokenTable[key] = tokentype;
}

std::string tokenTypeToString(const tokenType type) {
    switch (type) {
        case tokenType::IF:
            return "IF";
            break;
        case tokenType::ELSE:
            return "ELSE";
            break;
        case tokenType::WHILE:
            return "WHILE";
            break;
        case tokenType::FOR:
            return "FOR";
            break;
        case tokenType::RETURN:
            return "RETURN";
            break;
        case tokenType::INT:
            return "INT";
            break;
        case tokenType::BOOL:
            return "BOOL";
            break;
        case tokenType::PLUS:
            return "PLUS";
            break;
        case tokenType::MINUS:
            return "MINUS";
            break;
        case tokenType::MULTIPLY:
            return "MULTIPLY";
            break;
        case tokenType::DIVIDE:
            return "DIVIDE";
            break;
        case tokenType::LG_AND:
            return "LG_AND";
            break;
        case tokenType::LG_OR:
            return "LG_OR";
            break;
        case tokenType::LG_NOT:
            return "LG_NOT";
            break;
        case tokenType::LG_EQUAL:
            return "LG_EQUAL";
            break;
        case tokenType::LG_N_EQUAL:
            return "LG_N_EQUAL";
            break;
        case tokenType::LG_GREATER:
            return "LG_GREATER";
            break;
        case tokenType::LG_LESS:
            return "LG_LESS";
            break;
        case tokenType::LG_GREATER_EQUAL:
            return "LG_GREATER_EQUAL";
            break;
        case tokenType::LG_LESS_EQUAL:
            return "LG_LESS_EQUAL";
            break;
        case tokenType::ASSIGNMENT:
            return "ASSIGNMENT";
            break;
        case tokenType::IDENTIFIER:
            return "IDENTIFIER";
            break;
        case tokenType::INT_LITERAL:
            return "INT_LITERAL";
            break;
        case tokenType::BOOL_TRUE:
            return "BOOL_TRUE";
            break;
        case tokenType::BOOL_FALSE:
            return "BOOL_FALSE";
            break;
        case tokenType::LPAREN:
            return "LPAREN";
            break;
        case tokenType::RPAREN:
            return "RPAREN";
            break;
        case tokenType::LBRACE:
            return "LBRACE";
            break;
        case tokenType::RBRACE:
            return "RBRACE";
            break;
        case tokenType::SEMICOLON:
            return "SEMICOLON";
            break;
        case tokenType::COMMA:
            return "COMMA";
            break;
        case tokenType::ENDOFFILE:
            return "ENDOFFILE";
            break;
        case tokenType::ERR:
            return "ERR";
            break;
        default:
            return "";
    }
}

VarType tokenTypeToVarType(const tokenType type) {
    switch (type) {
        case tokenType::INT:
            return VarType::INT;
        case tokenType::BOOL:
            return VarType::BOOL;
        case tokenType::VOID:
            return VarType::VOID;
        default:
            return VarType::UNKNOWN;
    }
}


