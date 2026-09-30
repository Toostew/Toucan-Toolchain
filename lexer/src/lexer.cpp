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

//short hand since we do it 3 seperate times
void Lexer::flushBuffer(std::string &buffer) {
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
}


void Lexer::analyzeFile(std::string inputFile, std::string outputFile) {
    std::string line;

    std::ofstream writeFile(outputFile);
    std::ifstream readFile(inputFile);

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
                    flushBuffer(buffer);
                    pos++;
                    continue;
                }

                //numbers are treated special, they must appear standalaone without any alphabet chars
                //numbers that have alphabet chars are treated as a string
                else if (std::isdigit(c)) {
                    flushBuffer(buffer);
                    buffer += c;
                    pos++;


                    //TODO: this current sub-while loop is word soup, can probably reimplement this better
                    while (pos < line.length()) {
                        c = line[pos];
                        tempString = {c};
                        type = tokenTable.getToken(tempString);
                        if (type == tokenType::ERR) {
                            //the current char is not in reserved keywords
                            if (std::isdigit(c)) {
                                //char is a number, add to buffer
                                buffer += c;

                            } else if (std::isspace(c)) {
                                //char is whitespace
                                addTokenToVector(buffer, tokenType::INT_LITERAL);
                                buffer = "";
                                break;


                            } else {
                                //char is any other character,
                                //treat the buffer as a string
                                buffer += c;
                                break;
                            }


                        } else {
                            //the char IS in reserved keywords
                            addTokenToVector(buffer, tokenType::INT_LITERAL); //the int buffer is stored itself
                            pos--; //since pos automatically increments we need it to start again from the same pos
                            buffer = "";
                            break;
                        }

                        pos++;
                    }
                    //if we reach EOL while still parsing numbers, we need to immadiately save it
                    if (buffer.length() > 0 && pos >= line.length()) {
                        addTokenToVector(buffer, tokenType::INT_LITERAL);
                        buffer = "";
                    }
                }


                //check if it's || or &&,
                else if ((pos + 1 < line.length()) && (c == '&' || c == '|')) {
                    if ((c == '&' && line[pos + 1] == '&')) {
                        flushBuffer(buffer);
                        pos++;
                        addTokenToVector("&&", tokenType::LG_AND);
                    }
                    else if ((c == '|' && line[pos + 1] == '|')) {
                        flushBuffer(buffer);
                        pos++;
                        addTokenToVector("||", tokenType::LG_OR);
                    } else {
                        buffer += c;
                    }
                }

                else {
                    //the char is any other character
                    buffer += c;
                }




            }
            //the char is part of the list of recognized tokens
            else {
                //if the buffer is not empty, add as type IDENTIFIER and flush
                flushBuffer(buffer);

                //for some tokens we need to check if the same token appears again, like ==, for =
                //currently, only =/==, !/!=, >/>= and </<= can have this issue
                if ((c == '=' || c == '>' || c == '<' || c == '!') && pos + 1 < line.length() && line[pos + 1] == '=') {
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

                }



                else {
                    //single char keyword, (+,-,*,/,=, punctuation)
                    addTokenToVector(tempString, type);
                }
            }

            pos++;
        }
        //we check if the buffer is populated, and save/flush it
        flushBuffer(buffer);

    }

}

