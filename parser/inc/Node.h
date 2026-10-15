//
// Created by tooka on 15/10/2026.
//

#ifndef NTR_TOOLCHAIN_NODE_H
#define NTR_TOOLCHAIN_NODE_H



#include <memory>
#include <vector>
#include <string>
#include <cstdint>
#include <iostream>

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


class Visitor;

class Node {
public:
    virtual ~Node() {};
    virtual void accept(Visitor& visitor) = 0; //pure virtual

};

//common type
class Expression : public Node {
public:
    VarType varType = VarType::UNKNOWN; //init default to unknown
};
class UnaryExpression : public Expression {};
class BinaryExpression : public Expression {};
class AssignExpression : public Expression {};
class Literal_BOOL : public Expression {
public:
    void accept(Visitor& visitor);
    bool value;
};
class Literal_INT : public Expression {
public:
    void accept(Visitor& visitor);
    int value;
};
class VarReference : public Expression {};
class Call : public Expression {};



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

    void accept(Visitor& visitor);
};

class Function : public Node {
public:
    Block functionBlock;


    void accept(Visitor& visitor);
};

class Program : public Node {
public:
    std::vector<std::unique_ptr<Function>> functions;


    void accept(Visitor& visitor);
};

class Visitor {
public:
    virtual void visit(Block& block) = 0;
    virtual void visit(Function& function) = 0;
    virtual void visit(Program& program) = 0;
    //statement
    virtual void visit(IfStatement& ifStatement) = 0;
    virtual void visit(ForStatement& forStatement) = 0;
    virtual void visit(WhileStatement& whileStatement) = 0;
    virtual void visit(ReturnStatement& returnStatement) = 0;
    virtual void visit(ExpressStatement& expressStatement) = 0;
    virtual void visit(VarDeclarationStatement& varDeclarationStatement) = 0;
    //expression
    virtual void visit(UnaryExpression& unaryExpression) = 0;
    virtual void visit(BinaryExpression& binaryExpression) = 0;
    virtual void visit(AssignExpression& assignExpression) = 0;
    virtual void visit(Literal_BOOL& literalBool) = 0;
    virtual void visit(Literal_INT& literalInt) = 0;
    virtual void visit(VarReference& varReference) = 0;
    virtual void visit(Call& call) = 0;


    virtual ~Visitor() = default;
};

//print into console
class dumpVisitor : public Visitor {
public:
    virtual void visit(Block& block);
    virtual void visit(Function& function);
    virtual void visit(Program& program);
    //statement
    virtual void visit(IfStatement& ifStatement);
    virtual void visit(ForStatement& forStatement);
    virtual void visit(WhileStatement& whileStatement);
    virtual void visit(ReturnStatement& returnStatement);
    virtual void visit(ExpressStatement& expressStatement);
    virtual void visit(VarDeclarationStatement& varDeclarationStatement);
    //expression
    virtual void visit(UnaryExpression& unaryExpression);
    virtual void visit(BinaryExpression& binaryExpression);
    virtual void visit(AssignExpression& assignExpression);
    virtual void visit(Literal_BOOL& literalBool);
    virtual void visit(Literal_INT& literalInt);
    virtual void visit(VarReference& varReference);
    virtual void visit(Call& call);

    virtual ~dumpVisitor() = default;
};

#endif //NTR_TOOLCHAIN_NODE_H