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

class Visitor;
class Block;
class Expression;
class IfStatement;
class VarDeclarationStatement;

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

enum class IfStatementType {
    IF,
    IFELSE,
    IFELSEIF
};

typedef struct {
    IfStatementType type;
    std::unique_ptr<Expression> ifCondition;
    std::unique_ptr<Block> ifBlock;
    std::unique_ptr<Expression> elseBlock;
    std::unique_ptr<IfStatement> elseIf;
} IfStatementContent;


typedef struct {
    VarType varType;
    std::string name;
} Param;



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
class UnaryExpression : public Expression {
public:
    UnaryOperator op;
    std::unique_ptr<Expression> operand;

    void accept(Visitor& visitor) override;
    UnaryExpression(UnaryOperator op, std::unique_ptr<Expression> operand);
};
class BinaryExpression : public Expression {
public:
    BinaryOperator op;
    std::unique_ptr<Expression> leftOperand;
    std::unique_ptr<Expression> rightOperand;

    void accept(Visitor& visitor) override;
    BinaryExpression(BinaryOperator op, std::unique_ptr<Expression> leftOperand, std::unique_ptr<Expression> rightOperand);
};
class AssignExpression : public Expression {
public:
    std::string target;
    std::unique_ptr<Expression> expression;

    void accept(Visitor& visitor) override;
    AssignExpression(std::string target, std::unique_ptr<Expression> expression);
};
class Literal_BOOL : public Expression {
public:
    bool value;

    void accept(Visitor& visitor) override;
    Literal_BOOL(bool value);
};
class Literal_INT : public Expression {
public:
    int value;

    void accept(Visitor& visitor) override;
    Literal_INT(int value);
};
class VariableReference : public Expression {
public:
    std::string varName;

    void accept(Visitor& visitor) override;
    VariableReference(std::string varName);
};
class Call : public Expression {
public:
    std::string functionName;
    std::vector<std::unique_ptr<Expression>> arguments;

    void accept(Visitor& visitor) override;
    Call(std::string name, std::vector<std::unique_ptr<Expression>> arguments);
};



//common type
class Statement : public Node {
};

class IfStatement : public Statement {
public:
    IfStatementType ifStatementType;
    IfStatementContent ifStatementContent;

    void accept(Visitor& visitor) override;
    IfStatement(IfStatementType ifStatementType, IfStatementContent ifStatementContent);
};
class WhileStatement : public Statement {
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Block> block;


};
class ForStatement : public Statement {
    std::unique_ptr<VarDeclarationStatement> forInit;
    std::unique_ptr<Expression> forCondition;
    std::unique_ptr<Expression> forIncrement;
    std::unique_ptr<Block> block;

    void accept(Visitor &visitor) override;
    ForStatement(std::unique_ptr<VarDeclarationStatement> forInit, std::unique_ptr<Expression> forCondition,std::unique_ptr<Expression> forIncrement, std::unique_ptr<Block> block);
};
class ReturnStatement : public Statement {
    VarType returnType;
    std::unique_ptr<Expression> returnExpression;

    void accept(Visitor& visitor) override;
    ReturnStatement(VarType returnType, std::unique_ptr<Expression> returnExpression);
};
class ExpressStatement : public Statement {
    std::unique_ptr<Expression> expression;

    void accept(Visitor& visitor) override;
    ExpressStatement(std::unique_ptr<Expression> expression);
};
class VarDeclarationStatement : public Statement {
    VarType varType;
    std::string varName;

    void accept(Visitor& visitor) override;
    VarDeclarationStatement(VarType varType, std::string varName);
};

class Block : public Node {
public:
    std::vector<std::unique_ptr<Statement>> statements;

    void accept(Visitor& visitor) override;
    Block(std::vector<std::unique_ptr<Statement>> statements);
};

class Function : public Node {
public:
    Block functionBlock;
    std::vector<Param> parameters;

    void accept(Visitor& visitor) override;
    Function(Block functionBlock, std::vector<Param> parameters);
};

class Program : public Node {
public:
    std::vector<std::unique_ptr<Function>> functions;


    void accept(Visitor& visitor) override;
    Program(std::vector<std::unique_ptr<Function>> functions);
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
    virtual void visit(VariableReference& variableReference) = 0;
    virtual void visit(Call& call) = 0;


    virtual ~Visitor() = default;
};

//print into console
class DumpVisitor : public Visitor {
public:
    virtual void visit(Block& block) override;
    virtual void visit(Function& function) override;
    virtual void visit(Program& program) override;
    //statement
    virtual void visit(IfStatement& ifStatement) override;
    virtual void visit(ForStatement& forStatement) override;
    virtual void visit(WhileStatement& whileStatement) override;
    virtual void visit(ReturnStatement& returnStatement) override;
    virtual void visit(ExpressStatement& expressStatement) override;
    virtual void visit(VarDeclarationStatement& varDeclarationStatement) override;
    //expression
    virtual void visit(UnaryExpression& unaryExpression) override;
    virtual void visit(BinaryExpression& binaryExpression) override;
    virtual void visit(AssignExpression& assignExpression) override;
    virtual void visit(Literal_BOOL& literalBool) override;
    virtual void visit(Literal_INT& literalInt) override;
    virtual void visit(VariableReference& variableReference) override;
    virtual void visit(Call& call) override;

    virtual ~DumpVisitor() = default;
};

#endif //NTR_TOOLCHAIN_NODE_H