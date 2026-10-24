//
// Created by tooka on 05/10/2026.
//

#ifndef PARSER_MAIN_H
#define PARSER_MAIN_H

#include <vector>
#include <stdio.h>
#include <string>
#include <utility>
#include <memory>
#include <fstream>
#include <iostream>
#include "TokenTable.h"
#include "parser.h"
#include <sstream>


std::vector<token> scanFileForTokens(const std::string& fileName);

int main(int argc, char* argv[]);




#endif