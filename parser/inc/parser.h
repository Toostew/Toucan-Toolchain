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
    uint32_t pos;



    std::unique_ptr<Program> parseProgram();
    std::unique_ptr<Function> parseFunction();
    std::unique_ptr<Block> parseBlock();
    std::unique_ptr<VarDeclarationStatement> parseVarDeclarationStatement();
    std::unique_ptr<IfStatement> parseIfStatement();
    std::unique_ptr<WhileStatement> parseWhileStatement();
    std::unique_ptr<ForStatement> parseStatement();
    std::unique_ptr<ReturnStatement> parseReturnStatement();
    std::unique_ptr<ExpressStatement> parseExpressStatement();


    std::unique_ptr<UnaryExpression> parseUnaryExpression();
    std::unique_ptr<BinaryExpression> parseBinaryExpression();
    std::unique_ptr<AssignExpression> parseAssignExpression();
    std::unique_ptr<Literal_BOOL> parseLiteral_BOOL();
    std::unique_ptr<Literal_INT> parseLiteral_INT();
    std::unique_ptr<VariableReference> parseVariableReference();
    std::unique_ptr<Expression> parseCall();

    token peek();
    bool check(const tokenType& type);
    void advance();
    void expect(const tokenType& type);

    explicit Parser(std::vector<token> tokens)
        :tokens(std::move(tokens)){};
};


#endif