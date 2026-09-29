//
// Created by tooka on 29/09/2026.
//

#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <stdio.h>
#include <string>
#include <utility>

#include "TokenTable.h"




class Lexer {
    public:
        void addToken(std::pair<std::string, tokenType> tokenPair); //this is a one way operation



    private:
        std::vector<std::pair<std::string, tokenType>> fileTokens;
};


#endif //NTR_TOOLCHAIN_LEXER_H