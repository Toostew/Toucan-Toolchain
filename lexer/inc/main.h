//
// Created by tooka on 29/09/2026.
//

#ifndef LEXER_MAIN_H
#define LEXER_MAIN_H

#include <vector>
#include <stdio.h>
#include <string>
#include <utility>
#include <memory>

#include "TokenTable.h"
#include "lexer.h"



std::string tokenTypeToString(token token);
void outputWriter(const std::string& outputFileName, const std::vector<token>& tokens);

int main(int argc, char* argv[]);

#endif //NTR_TOOLCHAIN_MAIN_H