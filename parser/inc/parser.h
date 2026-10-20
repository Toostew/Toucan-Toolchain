//
// Created by tooka on 06/10/2026.
//

#ifndef PARSER_H
#define PARSER_H
#include <cstdint>
#include <vector>
#include <stdio.h>
#include <string>
#include <utility>
#include <memory>
#include "TokenTable.h"

class Parser {
public:
    std::vector<token> tokens;
    uint32_t pos;


    token peek();
    bool check(const tokenType& type);
    void advance();
    void expect(const tokenType& type);

    explicit Parser(std::vector<token> tokens)
        :tokens(std::move(tokens)){};
};


#endif