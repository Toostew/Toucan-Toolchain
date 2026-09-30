//
// Created by tooka on 29/09/2026.
//

#include "lexer.h"




void Lexer::addTokenToVector(std::string text, tokenType type) {
    token tempToken;
    tempToken.text = text;
    tempToken.type = type;
    fileTokens.push_back(tempToken);
}

void Lexer::analyzeFile(std::string inputFile, std::string outputFile) {
    std::string line;

    std::ofstream writeFile(outputFile);
    std::ifstream readFile(inputFile);
    TokenTable tokenTable;

    if (!readFile.is_open()) {
        //file failed to open, error out
        std::cerr << "Error opening file: " << inputFile << std::endl;
        exit(20); //code 20 for file issues
    }

    while (std::getline(readFile, line)) {

        size_t pos = 0;
        std::string buffer = "";

        while (pos < line.length()) {
            char c = line[pos];
            std::string tempString = {c};
            tokenType type = tokenTable.getToken(tempString);
            if (type == tokenType::ERR) {
                //the char is not in the list of tokens
                //it could be whitespace, in that case, skip. Otherwise append
                //we cast to unsigned char to avoid undefined behaviour on certain symbols
                if (std::isspace(static_cast<unsigned char>(c))) {
                    //if the char is whitespace, check buffer and savbe if populated
                    if (buffer.length() > 0) {
                        tokenType tempType = tokenTable.getToken(buffer);
                        if (tempType == tokenType::ERR) {
                            //key not found, hence is an identifier
                            addTokenToVector(buffer, tokenType::IDENTIFIER);

                        } else {
                            //key found, valid keyword
                            addTokenToVector(buffer, tempType);
                        }
                    }
                    buffer = "";
                    pos++;
                    continue;
                }


                buffer += tempString;

            } else {
                //the char is part of the list of tokens, admit here
                //if the buffer is not empty, add as type IDENTIFIER and flush
                if (buffer.length() > 0) {
                    tokenType tempType = tokenTable.getToken(buffer);
                    if (tempType == tokenType::ERR) {
                        //key not found, hence is an identifier
                        addTokenToVector(buffer, tokenType::IDENTIFIER);

                    } else {
                        //key found, valid keyword
                        addTokenToVector(buffer, tempType);
                    }
                    buffer = ""; //flush it
                }


                //for some tokens we need to check if the same token appears again, like ==, for =
                //currently, only =/==, !/!=, >/>= and </<= can have this issue
                if (pos + 1 < line.length() && line[pos + 1] == '=') {
                    switch (c) {
                        case '=':
                            addTokenToVector("==", tokenType::LG_EQUAL);
                            pos++;
                            break;
                        case '>':
                            addTokenToVector(">=", tokenType::LG_GREATER_EQUAL);
                            pos++;
                            break;
                        case '<':
                            addTokenToVector("<=", tokenType::LG_LESS_EQUAL);
                            pos++;
                            break;
                        case '!':
                            addTokenToVector("!=", tokenType::LG_N_EQUAL);
                            pos++;
                            break;
                    }

                } else {
                    //single char keyword, (+,-,*,/,=, punctuation)
                    addTokenToVector(tempString, type);
                }
            }

            pos++;
        }
        //we check if the buffer is populated, and save/flush it
        if (buffer.length() > 0) {
            tokenType tempType = tokenTable.getToken(buffer);
            if (tempType == tokenType::ERR) {
                //key not found, hence is an identifier
                addTokenToVector(buffer, tokenType::IDENTIFIER);

            } else {
                //key found, valid keyword
                addTokenToVector(buffer, tempType);
            }
        }

    }

}

