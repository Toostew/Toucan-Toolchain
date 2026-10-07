//
// Created by tooka on 05/10/2026.
//

#include "main.h"




int main(int argc, char* argv[]) {


    return 0;
}


//for the vast majority of data structs data is already on the heap.

std::vector<token> scanFileForTokens(const std::string& fileName) {
    std::ifstream inputFile(fileName);




    int currLineNum = 0;

    if (!inputFile.is_open()) {
        std::cerr << "Error opening file " << fileName << std::endl;
        exit(20); //code 20 is for file opening issues
    }
    TokenTable tokenTable;
    std::string inputFileBuffer;
    std::string issBuffer;
    std::vector<token> fileTokens;


    //go line by line
    while (std::getline(inputFile, inputFileBuffer)) {
        currLineNum++;
        std::istringstream inputStream(inputFileBuffer);
        token lineToken;

        //istringstream reads strings delimmited by spaces, which we designed around this logic
        //currently our expected input is always [TYPE] [CONTENT]
        //if the count is lower than expected, error out. if higher, ignore
        for (int i = 0; i < 2; i++) {
            if (!(inputStream >> issBuffer)) {
                //less than 2 substrings, error out
                std::cerr << "Not enough Directives at line:   " << currLineNum << std::endl;
                exit(30); //code 30 is for issues in directive detection
            }
            if (i == 0) {
                tokenType checkTokenType = tokenTable.getToken(issBuffer);
                if (checkTokenType != tokenType::ERR) {
                    //known tokentype
                    lineToken.type = checkTokenType;

                } else {
                    //ERR, shouldn't be here
                    //add anyway for processing later
                    std::cerr << "Unknown directive at line:   " << currLineNum << std::endl;
                    lineToken.type = tokenType::ERR;
                }
            }
            else {
                lineToken.text = issBuffer; //content part
            }

        }
        fileTokens.push_back(lineToken);
    }



    return fileTokens;
    //most data structs provided by cpp are move aware and follow Named Return Value Optimization (NRVO)
    //when a function returns a named local variable, the compiler is PERMITTED to construct it directly in
    //the caller's destination storage, so on the callee's termination, no copying or moving is needed at all.
    //So an std:move() isnt actually a good idea here, since it disables NRVO and performs a move
}

