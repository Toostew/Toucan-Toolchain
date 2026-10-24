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
#include "Node.h"

class Parser {
public:
    std::vector<token> tokens;
    size_t pos = 0;



    std::unique_ptr<Program> parseProgram();
    std::unique_ptr<Function> parseFunction();
    Param parseParam();
    std::unique_ptr<Block> parseBlock();
    std::unique_ptr<VarDeclarationStatement> parseVarDeclarationStatement();
    std::unique_ptr<IfStatement> parseIfStatement();
    std::unique_ptr<WhileStatement> parseWhileStatement();
    std::unique_ptr<ForStatement> parseForStatement();
    std::unique_ptr<ReturnStatement> parseReturnStatement();
    std::unique_ptr<ExpressStatement> parseExpressStatement();

    std::unique_ptr<Expression> parseExpression();
    std::unique_ptr<Expression> parseLgOR();
    std::unique_ptr<Expression> parseLgAND();
    std::unique_ptr<Expression> parseLgEquality();
    std::unique_ptr<Expression> parseLgComparator();
    std::unique_ptr<Expression> parsePlusMinus();
    std::unique_ptr<Expression> parseMultiplication();
    std::unique_ptr<Expression> parseUnary();
    std::unique_ptr<Expression> parseBase();






    token peek();
    bool check(const tokenType& type);
    bool check(const std::vector<tokenType>& types);
    void advance();
    token expect(const tokenType& type);
    token expect(const std::vector<tokenType>& types);

    explicit Parser(std::vector<token>& tokens)
        :tokens(std::move(tokens)){};
};


#endif