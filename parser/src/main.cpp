//
// Created by tooka on 05/10/2026.
//

#include "main.h"

#include <iostream>


int main(int argc, char* argv[]) {


    return 0;
}


std::vector<std::string> scanFileForTokens(std::string fileName) {
    std::ifstream inputFile(fileName);
    std::string buffer;

    if (!inputFile.is_open()) {
        std::cerr << "Error opening file " << fileName << std::endl;
        exit(20);
    }

    //go line by line
    while (std::getline(inputFile, buffer)) {

    }



}

