//
// Created by tooka on 05/10/2026.
//

#include "main.h"
#include "Node.h"



int main(int argc, char* argv[]) {
    DumpVisitor dump;

    std::unique_ptr<Expression> exampleA = std::make_unique<Literal_INT>(200);
    std::unique_ptr<Expression> exampleB = std::make_unique<Literal_BOOL>(false);
    std::unique_ptr<Expression> exampleC = std::make_unique<Literal_BOOL>(true);

    std::vector<std::unique_ptr<Statement>> ifBlockStatementsOne;
    std::vector<std::unique_ptr<Statement>> ifBlockStatementsTwo;
    std::unique_ptr<Block> ifBlockOne = std::make_unique<Block>(std::move(ifBlockStatementsOne));
    std::unique_ptr<Block> ifBlockTwo = std::make_unique<Block>(std::move(ifBlockStatementsTwo));


    std::unique_ptr<Statement> exampleAStatement = std::make_unique<ExpressStatement>(std::move(exampleA));
    std::unique_ptr<IfStatement> exampleCStatement = std::make_unique<IfStatement>(IfStatementType::IF, std::move(exampleC), std::move(ifBlockTwo), nullptr, nullptr);
    std::unique_ptr<Statement> exampleDStatement = std::make_unique<IfStatement>(IfStatementType::IFELSEIF, std::move(exampleB),std::move(ifBlockOne),nullptr,std::move(exampleCStatement));



    std::vector<std::unique_ptr<Statement>> statements;
    statements.push_back(std::move(exampleAStatement));
    statements.push_back(std::move(exampleDStatement));


    std::unique_ptr<Block> block = std::make_unique<Block>(std::move(statements));

    Param pOne = {VarType::BOOL,"truthness"};
    Param pTwo = {VarType::INT,"hardness"};
    std::vector<Param> parameters = {pOne, pTwo};

    std::unique_ptr<Function> funcX = std::make_unique<Function>("funcX", *block,parameters,VarType::VOID);

    std::vector<std::unique_ptr<Function>> functions;
    functions.push_back(std::move(funcX));



    Program start = Program(std::move(functions));

    dump.visit(start);
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
    //which is actually worse than just letting it return raw value
}

