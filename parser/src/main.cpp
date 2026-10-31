//
// Created by tooka on 05/10/2026.
//

#include "main.h"
#include "Node.h"
#include "Visitor.h"








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

        //istringstream reads strings delimited by spaces, which we designed around this logic
        //currently our expected input is always [TYPE] [CONTENT]
        //if the count is lower than expected, error out. if higher, ignore
        for (int i = 0; i < 3; i++) {
            if (!(inputStream >> issBuffer)) {
                //less than 2 substrings, error out
                std::cerr << "Not enough Directives at lexer file line:   " << currLineNum << std::endl;
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
                    std::cerr << "Unknown directive " << issBuffer << " at lexer file line: " << currLineNum << std::endl;
                    lineToken.type = tokenType::ERR;
                }
            }
            else if (i == 1){
                lineToken.text = issBuffer; //content part
            }
            else {
                try {
                    //Convert to unsigned long long, 2. Safely cast to size_t
                    lineToken.line = static_cast<size_t>(std::stoull(issBuffer));
                }
                catch (const std::invalid_argument& e) {
                    std::cerr << "Invalid argument: Not a valid number." << std::endl;
                }
                catch (const std::out_of_range& e) {
                    std::cerr << "Out of range: Number is too large." << std::endl;
                }
            }

        }
        fileTokens.push_back(lineToken);
    }

    token endToken = {"",tokenType::ENDOFFILE};

    fileTokens.push_back(endToken); //add ENDOFFILE check


    return fileTokens;
    //most data structs provided by cpp are move aware and follow Named Return Value Optimization (NRVO)
    //when a function returns a named local variable, the compiler is PERMITTED to construct it directly in
    //the caller's destination storage, so on the callee's termination, no copying or moving is needed at all.
    //So an std:move() isnt actually a good idea here, since it disables NRVO and performs a move
    //which is actually worse than just letting it return raw value
}






int main(int argc, char* argv[]) {

    if (argc < 2 || argc > 2) {
        std::cerr << "parser <filename>" << std::endl;
    } else {
        std::vector<token> tokens = scanFileForTokens(argv[1]);
        Parser parser = Parser(tokens);
        std::unique_ptr<Program> program = parser.parseProgram();

        DumpVisitor dumpVisitor = DumpVisitor();
        dumpVisitor.visit(*program);

    }


    return 0;
}




