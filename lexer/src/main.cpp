//
// Created by tooka on 29/09/2026.
//

#include "main.h"





void outputWriter(const std::string& outputFileName, const std::vector<token>& tokens) {

    std::ofstream outputFile(outputFileName);

    for (const token& tokenEntry : tokens) {
        outputFile << tokenTypeToString(tokenEntry) << " " << tokenEntry.text << "\n";
    }
}


std::string tokenTypeToString(token token) {
    std::string result;
    switch (token.type) {
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
        default:
            return "UNKNOWN";
            break;
    }
}

int main() {
    Lexer lexer;
    lexer.analyzeFile("lexerTesterIN.txt");
    outputWriter("lexerTesterOUT.txt", lexer.getFileTokens());

    return 0;
}




