//
// Created by tooka on 29/09/2026.
//

#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <stdio.h>
#include <string>
#include <utility>
#include <memory>

#include "TokenTable.h"


class Lexer {
    public:
        void addTokenToVector(std::string text, tokenType type); //add token to the vector
        void analyzeFile(std::string inputFile);
        std::vector<token> getFileTokens();

    private:
        TokenTable tokenTable;
        std::vector<token> fileTokens; //list, in order, of tokens and their appearance in the file
        void flushBuffer(std::string& buffer);
};




#endif //LEXER_H