//
// Created by tooka on 06/10/2026.
//


#include "parser.h"

#include <cmath>

#include "exceptions.h"

std::unique_ptr<Program> Parser::parseProgram() {
    std::vector<std::unique_ptr<Function>> functions;

    while (!check(tokenType::ENDOFFILE)) {
        functions.push_back(parseFunction());
    }

    if (functions.size() == 0) {
        throw noFunctions("No functions declared in program, must have at least 1");
    }

    return std::make_unique<Program>(std::move(functions));
}

std::unique_ptr<Function> Parser::parseFunction() {
    //[return type] [function name] [parameters] [block]
    //if something is an int or bool we are not sure if its a function declaration
    //or a var declaration, need more info
    //functions always have () for parameters, must check for that
    //v1 doesn't support global vars

    //vartype
    VarType varType;
    token currToken = peek();
    if ((currToken.type == tokenType::BOOL) || (currToken.type == tokenType::INT) || (currToken.type == tokenType::VOID)) {
       varType = tokenTypeToVarType(currToken.type);
    } else {
        //expected varType, error
        throw IncorrectTokenType("Expected int, bool or void, got " + tokenTypeToString(currToken.type));
    }
    advance();

    //identifier
    std::string identifier;
    currToken = peek();
    if (check(tokenType::IDENTIFIER)) {
        identifier = currToken.text;
    } else {
        throw IncorrectTokenType("Expected identifier, got " + tokenTypeToString(currToken.type));
    }
    advance();

    //LParen
    //if it's not LParen, chances are this was a vardeclaration
    expect(tokenType::LPAREN);
    advance();

    //parameter(s)
    std::vector<Param> params;
    while (!(check(tokenType::RPAREN))) {
        //[VarType] [identifier] [OPTIONAL: comma]
        params.push_back(parseParam());


        //if there is a comma, expect another Param
        if (check(tokenType::COMMA)) {
            advance();
        } else {
            //if there is no comma it must be the end
            expect(tokenType::RPAREN);
        }
    }
    advance();

    return std::make_unique<Function>(identifier,*parseBlock(),params,varType);

}

Param Parser::parseParam() {
    //[VarType] [identifier] [OPTIONAL: comma]
    Param currParam;
    currParam.varType = tokenTypeToVarType(expect(std::vector<tokenType>({tokenType::INT, tokenType::BOOL})).type);
    advance();
    currParam.name = expect(tokenType::IDENTIFIER).text;
    advance();

    return currParam;
}

std::unique_ptr<Block> Parser::parseBlock() {
    //[{] [statements]* [}]
    std::vector<std::unique_ptr<Statement>> statements;
    expect(tokenType::LBRACE);
    advance();
    while (!check(tokenType::RBRACE)) {
        token currToken = peek();
        switch (currToken.type) {
            case tokenType::IF:
                statements.push_back(parseIfStatement());
                break;
            case tokenType::WHILE:
                statements.push_back(parseWhileStatement());
                break;
            case tokenType::FOR:
                statements.push_back(parseForStatement());
            case tokenType::RETURN:
                statements.push_back(parseReturnStatement());
            case tokenType::BOOL:
                statements.push_back(parseVarDeclarationStatement());
            case tokenType::INT:
                statements.push_back(parseVarDeclarationStatement());
            case tokenType::IDENTIFIER:
                //could be a call, either func call or normal


        }


    }
}










token Parser::peek() {
    return tokens[pos];
}

bool Parser::check(const tokenType& type) {
    return Parser::peek().type == type;
}

bool Parser::check(const std::vector<tokenType>& types) {
    for (const tokenType& type : types) {
        if (peek().type == type) {
            return true;
        }
    }
    return false;
}

void Parser::advance() {
    if (!(peek().type == tokenType::ENDOFFILE)) {
        pos++;
    } else {
        //error handling: EOL
        throw OutOfFile("Reached end of file before completing parsing");
    }

}

//expect does not advance
token Parser::expect(const tokenType& type) {
    token currToken = peek();
    if (currToken.type == type) {
        return currToken;
    } else {
        //error handling here: not correct type
        std::string message = "Expected " + tokenTypeToString(type) +", got " + tokenTypeToString(currToken.type);
        throw IncorrectTokenType(message);
    }

}

token Parser::expect(const std::vector<tokenType>& types) {
    token currToken = peek();
    for (const tokenType& type : types) {
        if (currToken.type == type) {
            return currToken;
        }
    }

    std::string expectedTokens = "";
    for (int i = 0; i < types.size(); i++) {
        if (i != types.size() - 1) {
            expectedTokens += tokenTypeToString(types[i]) + ", ";
        } else {
            expectedTokens += tokenTypeToString(types[i]);
        }
    }

    throw IncorrectTokenType("expected " + expectedTokens + ", got " + tokenTypeToString(currToken.type));
}






