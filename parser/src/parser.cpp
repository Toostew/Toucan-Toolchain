//
// Created by tooka on 06/10/2026.
//


#include "parser.h"
#include <algorithm>
#include "exceptions.h"

std::unique_ptr<Program> Parser::parseProgram() {
    //by default, there is only one Program, and it is always line 0
    std::vector<std::unique_ptr<Function>> functions;

    while (!check(tokenType::ENDOFFILE)) {
        functions.push_back(parseFunction());
    }

    if (functions.size() == 0) {
        throw noFunctions("No functions declared in program, must have at least 1");
    }

    return std::make_unique<Program>(std::move(functions),std::pair<size_t,size_t>(0,0));
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
    size_t startLine;
    if ((currToken.type == tokenType::BOOL) || (currToken.type == tokenType::INT) || (currToken.type == tokenType::VOID)) {
       varType = tokenTypeToVarType(currToken.type);
        startLine = currToken.line;
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
    if (!(check(tokenType::RPAREN))) {
        params.push_back(parseParam());

        while (check(tokenType::COMMA)) {
            advance();
            params.push_back(parseParam());
        }
    }
    expect(tokenType::RPAREN);
    advance();



    std::unique_ptr<Block> block = parseBlock();

    size_t endLine = block->lines.second;

    return std::make_unique<Function>(identifier,std::move(*block),params,varType, std::pair<size_t,size_t>(startLine,endLine));

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
    size_t startLine = expect(tokenType::LBRACE).line;
    advance();
    while (!check(tokenType::RBRACE) && !check(tokenType::ENDOFFILE)) {
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
                break;
            case tokenType::RETURN:
                statements.push_back(parseReturnStatement());
                break;
            case tokenType::BOOL: case tokenType::INT:
                statements.push_back(parseVarDeclarationStatement());
                break;
            case tokenType::IDENTIFIER:
                //could be a call, either func call or normal
                statements.push_back(parseExpressStatement());
                break;
            default:
                statements.push_back(parseExpressStatement());
                break;
        }

    }
    size_t endLine = expect(tokenType::RBRACE).line;
    advance();


    return std::make_unique<Block>(std::move(statements), std::pair<size_t,size_t>(startLine,endLine));
}

std::unique_ptr<VarDeclarationStatement> Parser::parseVarDeclarationStatement() {
    //[varType] [identifier] [OPTIONAL [assignment] [expression] ]
    VarType varType = tokenTypeToVarType(expect(std::vector<tokenType>({tokenType::INT, tokenType::BOOL})).type);
    advance();
    std::string identifier = expect(tokenType::IDENTIFIER).text;
    size_t line = expect(tokenType::IDENTIFIER).line;
    advance();
    std::unique_ptr<Expression> expression = nullptr;

    //init is REQUIRED for var declarations
    expect(tokenType::ASSIGNMENT);
    advance();
    expression = std::move(parseExpression()); //
    expect(tokenType::SEMICOLON);
    advance();


    /*
    if (check(tokenType::ASSIGNMENT)) {
        advance();
        expression = std::move(parseExpression()); //
        expect(tokenType::SEMICOLON);
        advance();
    } else {
        expect(tokenType::SEMICOLON);
        advance();
    }
    */

    return std::make_unique<VarDeclarationStatement>(varType, identifier, std::move(expression), std::pair<size_t,size_t>(line,line));
}

std::unique_ptr<IfStatement> Parser::parseIfStatement() {
    //[if] [(] [expression] [)] [block] [OPTIONAL: [ELSE] [IF]]
    size_t startLine = expect(tokenType::IF).line;
    advance();
    expect(tokenType::LPAREN);
    advance();
    std::unique_ptr<Expression> ifCondition = std::move(parseExpression());
    expect(tokenType::RPAREN);
    advance();
    std::unique_ptr<Block> ifBlock = std::move(parseBlock());

    size_t endLine = ifBlock->lines.second;

    //optional ELSE, ELSE IF
    IfStatementType ifStatementType;
    std::unique_ptr<IfStatement> ifStatement = nullptr;
    std::unique_ptr<Block> elseBlock = nullptr;
    if (check(tokenType::ELSE)) {
        advance();

        if (check(tokenType::IF)) {
            //IF ELSE IF
            ifStatement = std::move(parseIfStatement());
            ifStatementType = IfStatementType::IFELSEIF;
            endLine = ifStatement->lines.second;
        } else {
            //IF ELSE
            elseBlock = std::move(parseBlock());
            ifStatementType = IfStatementType::IFELSE;
            endLine = elseBlock->lines.second;
        }
    } else {
        // IF
        ifStatementType = IfStatementType::IF;
    }


    return std::make_unique<IfStatement>(ifStatementType,std::move(ifCondition),std::move(ifBlock), std::move(elseBlock), std::move(ifStatement), std::pair<size_t,size_t>(startLine,endLine));
}

std::unique_ptr<WhileStatement> Parser::parseWhileStatement() {
    //[while] [(] [expression] [)] [block]
    size_t startLine = expect(tokenType::WHILE).line;
    advance();
    expect(tokenType::LPAREN);
    advance();
    std::unique_ptr<Expression> whileCondition = std::move(parseExpression());
    expect(tokenType::RPAREN);
    advance();
    std::unique_ptr<Block> whileBlock = std::move(parseBlock());

    size_t endLine = whileBlock->lines.second;

    return std::make_unique<WhileStatement>(std::move(whileCondition),std::move(whileBlock), std::pair<size_t,size_t>(startLine,endLine));
}

std::unique_ptr<ForStatement> Parser::parseForStatement() {
    //[for] [(] [forinit] [;] [forCondition] [;] [forIncrement] [)] [block]
    size_t startLine = expect(tokenType::FOR).line;
    advance();
    expect(tokenType::LPAREN);
    advance();
    std::unique_ptr<VarDeclarationStatement> forInit = std::move(parseVarDeclarationStatement()); //already consumes semicolon
    std::unique_ptr<Expression> forCondition = std::move(parseExpression());
    expect(tokenType::SEMICOLON);
    advance();
    std::unique_ptr<Expression> forIncrement = std::move(parseExpression());
    expect(tokenType::RPAREN);
    advance();
    std::unique_ptr<Block> forBlock = std::move(parseBlock()); //parse functions always advances to first unconsumed token

    size_t endLine = forBlock->lines.second;

    return std::make_unique<ForStatement>(std::move(forInit),std::move(forCondition),std::move(forIncrement), std::move(forBlock), std::pair<size_t,size_t>(startLine,endLine));
}

std::unique_ptr<ReturnStatement> Parser::parseReturnStatement() {
    //[return] [OPTIONAL: [expression]] [;]
    //void returns can do return;, int and bool needs the varType
    size_t line = expect(tokenType::RETURN).line;
    advance();

    std::unique_ptr<Expression> returnValue = nullptr;
    if (!check(tokenType::SEMICOLON)) {
        returnValue = std::move(parseExpression());
    }
    expect(tokenType::SEMICOLON);
    advance();

    return std::make_unique<ReturnStatement>(std::move(returnValue), std::pair<size_t,size_t>(line,line));
}

std::unique_ptr<ExpressStatement> Parser::parseExpressStatement() {
    std::unique_ptr<Expression> expression = std::move(parseExpression());
    expect(tokenType::SEMICOLON);
    advance();

    size_t line = expression->lines.first;


    return std::make_unique<ExpressStatement>(std::move(expression), std::pair<size_t,size_t>(line,line));
}










//TODO: make line detection be able to detect multi-line expressions
std::unique_ptr<Expression> Parser::parseExpression() {
    std::unique_ptr<Expression> left = std::move(parseLgOR());

    //could be a variable reference, we need to catch a potential assignment
    if (check(tokenType::ASSIGNMENT)) {
        auto* target = dynamic_cast<VariableReference*>(left.get());
        if (target == nullptr) {
            throw InvalidAssignmentTarget("Invalid assignment target");
        }
        advance();
        std::unique_ptr<Expression> value = std::move(parseExpression()); //advances
        size_t line = value->lines.first; //NOTE: the grammar allows for multilined expressions, this current line detection assumes expressions are made in one line
        return std::make_unique<AssignExpression>(target->varName, std::move(value), std::pair<size_t,size_t>(line,line));
    }
    return left;
}

std::unique_ptr<Expression> Parser::parseLgOR() {
    std::unique_ptr<Expression> left = std::move(parseLgAND());

    //this will solve parsing sequential ORs
    while (check(tokenType::LG_OR)) {
        advance();
        std::unique_ptr<Expression> right = std::move(parseLgAND());
        size_t line = right->lines.first;
        left = std::make_unique<BinaryExpression>(BinaryOperator::OR, std::move(left), std::move(right), std::pair<size_t,size_t>(line,line));
    }

    return left;
}

std::unique_ptr<Expression> Parser::parseLgAND() {
    std::unique_ptr<Expression> left = std::move(parseLgEquality());

    //solves sequential ANDs
    while (check(tokenType::LG_AND)) {
        advance();
        std::unique_ptr<Expression> right = std::move(parseLgEquality());
        size_t line = right->lines.first;
        left = std::make_unique<BinaryExpression>(BinaryOperator::AND, std::move(left), std::move(right), std::pair<size_t,size_t>(line,line));
    }
    return left;
}

std::unique_ptr<Expression> Parser::parseLgEquality() {
    std::unique_ptr<Expression> left = std::move(parseLgComparator());

    //solves sequential equality checks
    while (check(std::vector<tokenType>({tokenType::LG_EQUAL, tokenType::LG_N_EQUAL}))) {
        token curr = peek();
        advance();
        std::unique_ptr<Expression> right = std::move(parseLgComparator()); //advances
        size_t line = right->lines.first;

        if (curr.type == tokenType::LG_EQUAL) {
            left = std::make_unique<BinaryExpression>(BinaryOperator::EQUAL, std::move(left), std::move(right), std::pair<size_t,size_t>(line,line));
        } else {
            left = std::make_unique<BinaryExpression>(BinaryOperator::NOTEQUAL, std::move(left), std::move(right), std::pair<size_t,size_t>(line,line));
        }

    }
    return left;
}

std::unique_ptr<Expression> Parser::parseLgComparator() {
    std::unique_ptr<Expression> left = std::move(parsePlusMinus());

    //solves sequential comparators
    while (check(std::vector<tokenType>({tokenType::LG_GREATER, tokenType::LG_GREATER_EQUAL, tokenType::LG_LESS, tokenType::LG_LESS_EQUAL}))) {
        token curr = peek();
        advance();
        std::unique_ptr<Expression> right = std::move(parsePlusMinus()); //advances
        size_t line = right->lines.first;

        switch (curr.type) {
            case tokenType::LG_GREATER:
                left = std::make_unique<BinaryExpression>(BinaryOperator::MORETHAN, std::move(left), std::move(right), std::pair<size_t,size_t>(line,line));
                break;
            case tokenType::LG_GREATER_EQUAL:
                left = std::make_unique<BinaryExpression>(BinaryOperator::MORETHANOREQUAL, std::move(left), std::move(right), std::pair<size_t,size_t>(line,line));
                break;
            case tokenType::LG_LESS:
                left = std::make_unique<BinaryExpression>(BinaryOperator::LESSTHAN, std::move(left), std::move(right), std::pair<size_t,size_t>(line,line));
                break;
            case tokenType::LG_LESS_EQUAL:
                left = std::make_unique<BinaryExpression>(BinaryOperator::LESSTHANOREQUAL, std::move(left), std::move(right), std::pair<size_t,size_t>(line,line));
                break;
            default:
                //throw error, though this is not possible to reach
                break;
        }
    }
    return left;
}

std::unique_ptr<Expression> Parser::parsePlusMinus() {
    std::unique_ptr<Expression> left = std::move(parseMultiplication());
    size_t line = left->lines.first;

    //solves sequential plus and minus
    while (check(std::vector<tokenType>({tokenType::PLUS, tokenType::MINUS}))) {
        token curr = peek();
        advance();
        std::unique_ptr<Expression> right = std::move(parseMultiplication()); //advances

        if (curr.type == tokenType::PLUS) {
            left = std::make_unique<BinaryExpression>(BinaryOperator::PLUS, std::move(left), std::move(right), std::pair<size_t,size_t>(line,line));
        } else {
            //minus
            left = std::make_unique<BinaryExpression>(BinaryOperator::MINUS, std::move(left), std::move(right), std::pair<size_t,size_t>(line,line));
        }
    }

    return left;
}

std::unique_ptr<Expression> Parser::parseMultiplication() {
    std::unique_ptr<Expression> left = std::move(parseUnary());
    size_t line = left->lines.first;

    //solves sequential multiplications
    while (check(std::vector<tokenType>({tokenType::MULTIPLY, tokenType::DIVIDE}))) {
        token curr = peek();
        advance();
        std::unique_ptr<Expression> right = std::move(parseUnary()); //advances

        if (curr.type == tokenType::MULTIPLY) {
            left = std::make_unique<BinaryExpression>(BinaryOperator::MULTIPLY, std::move(left), std::move(right), std::pair<size_t,size_t>(line,line));
        } else {
            left = std::make_unique<BinaryExpression>(BinaryOperator::DIVIDE, std::move(left), std::move(right), std::pair<size_t,size_t>(line,line));
        }
    }
    return left;
}

std::unique_ptr<Expression> Parser::parseUnary() {
    //check for potential NOT or negative
    std::unique_ptr<Expression> operand;
    if (check(std::vector<tokenType>({tokenType::MINUS, tokenType::LG_NOT}))) {
        token curr = peek();
        advance();

        if (curr.type == tokenType::MINUS) {
            //MINUS
            operand = std::make_unique<UnaryExpression>(UnaryOperator::NEGATIVE, std::move(parseUnary()), std::pair<size_t,size_t>({curr.line,curr.line}));
        } else {
            //NOT
            operand = std::make_unique<UnaryExpression>(UnaryOperator::NOT, std::move(parseUnary()), std::pair<size_t,size_t>({curr.line,curr.line}));
        }


    } else {
        //no negative or NOT
        operand = std::move(parseBase());
    }

    return operand;
}

std::unique_ptr<Expression> Parser::parseBase() {
    token curr = peek();
    std::unique_ptr<Expression> base;
    std::vector<std::unique_ptr<Expression>> args;
    switch (curr.type) {
        case tokenType::INT_LITERAL:
            try  {
                std::stoi(curr.text);
            } catch (const std::out_of_range& e) {
                throw genericIssue("Int literal exceeds 32bit signed size limit");
            }
            base = std::make_unique<Literal_INT>(std::stoi(curr.text), std::pair<size_t,size_t>({curr.line,curr.line}));
            advance();
            break;
        case tokenType::BOOL_FALSE:
            base = std::make_unique<Literal_BOOL>(false, std::pair<size_t,size_t>({curr.line,curr.line}));
            advance();
            break;
        case tokenType::BOOL_TRUE:
            base = std::make_unique<Literal_BOOL>(true, std::pair<size_t,size_t>({curr.line,curr.line}));
            advance();
            break;
        case tokenType::IDENTIFIER:
            //either func or var
            advance();
            if (check(tokenType::LPAREN)) {
                advance();
                if (check(tokenType::RPAREN)) {
                    //no args
                    advance();
                    base = std::make_unique<Call>(curr.text, std::move(args), std::pair<size_t,size_t>({curr.line,curr.line}));
                } else {
                    //args
                    args.push_back(std::move(parseExpression()));
                    while (check(tokenType::COMMA)) {
                        advance();
                        args.push_back(std::move(parseExpression()));
                    }
                    expect(tokenType::RPAREN);
                    advance();
                    base = std::make_unique<Call>(curr.text, std::move(args), std::pair<size_t,size_t>({curr.line,curr.line}));
                }

            } else{
                //var reference
                base = std::make_unique<VariableReference>(curr.text, std::pair<size_t,size_t>({curr.line,curr.line}));
            }
            break;

        case tokenType::LPAREN:
            advance();
            base = std::move(parseExpression());
            expect(tokenType::RPAREN);
            advance();
            break;
        default:
            //error out
            throw IncorrectTokenType("Expected literal int or bool, function call or variable reference, got " + tokenTypeToString(curr.type));
            break;

    }
    return base;
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
    }

    else {
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

//overloaded for multiple types
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






