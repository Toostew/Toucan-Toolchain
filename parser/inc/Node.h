//
// Created by tooka on 15/10/2026.
//

#ifndef NTR_TOOLCHAIN_NODE_H
#define NTR_TOOLCHAIN_NODE_H



#include <memory>
#include <vector>
#include <string>
#include <cstdint>

//operators with 2 variables
enum class BinaryOperator{
    ADD,
    MINUS,
    MULTIPLY,
    DIVIDE,
    AND,
    OR,
    EQUAL,
    NOTEQUAL,
    MORETHAN,
    LESSTHAN,
    MORETHANOREQUAL,
    LESSTHANOREQUAL
};

enum class UnaryOperator {
    NOT,
    NEGATIVE
};

enum class StatementType {
    IF,
    WHILE,
    FOR,
    RETURN,
    EXPRESS,
    VARDECLARE
};

enum class VarType {
    INT,
    BOOL,
    VOID,
    UNKNOWN
};


typedef struct {
    VarType varType;
    std::string name;
} Param;

class Node {
public:
    virtual ~Node() {};

};

//common type
class Expression : public Node {
    public:
    VarType varType;
};
class UnaryExpression : public Expression {};
class BinaryExpression : public Expression {};
class AssignExpression : public Expression {};
class Literal_BOOL : public Expression {};
class Literal_INT : public Expression {};
class VarReference : public Expression {};
class call : public Expression {};



//common type
class Statement : public Node {
};

class IfStatement : public Statement {};
class WhileStatement : public Statement {};
class ForStatement : public Statement {};
class ReturnStatement : public Statement {};
class ExpressStatement : public Statement {};
class VarDeclarationStatement : public Statement {};

class Block : public Node {
public:
    std::vector<std::unique_ptr<Statement>> statements;


};

class Function : public Node {
public:
    Block functionBlock;

};

class Program : public Node {
public:
    std::vector<std::unique_ptr<Function>> functions;

};

#endif //NTR_TOOLCHAIN_NODE_H