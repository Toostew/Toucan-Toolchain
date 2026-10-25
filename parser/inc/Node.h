//
// Created by tooka on 15/10/2026.
//

#ifndef NTR_TOOLCHAIN_NODE_H
#define NTR_TOOLCHAIN_NODE_H



#include <memory>
#include <vector>
#include <string>
#include <cstdint>
#include <functional>
#include <iostream>

class Visitor;
class Block;
class Expression;
class IfStatement;


//operators with 2 variables
enum class BinaryOperator{
    PLUS,
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
class UnaryExpression : public Expression{
public:
    UnaryOperator op;
    std::unique_ptr<Expression> operand;

    void accept(Visitor& visitor) override;
    UnaryExpression(UnaryOperator op, std::unique_ptr<Expression> operand)
        :op(op), operand(std::move(operand)){};
};
class BinaryExpression : public Expression {
public:
    BinaryOperator op;
    std::unique_ptr<Expression> leftOperand;
    std::unique_ptr<Expression> rightOperand;

    void accept(Visitor& visitor) override;
    BinaryExpression(BinaryOperator op, std::unique_ptr<Expression> leftOperand, std::unique_ptr<Expression> rightOperand)
        :op(op), leftOperand(std::move(leftOperand)), rightOperand(std::move(rightOperand)){};
};
class AssignExpression : public Expression {
public:
    std::string target;
    std::unique_ptr<Expression> expression;

    void accept(Visitor& visitor) override;
    AssignExpression(std::string target, std::unique_ptr<Expression> expression)
        :target(std::move(target)), expression(std::move(expression)){};
};
class Literal_BOOL : public Expression {
public:
    bool value;

    void accept(Visitor& visitor) override;
    explicit Literal_BOOL(bool value) //explicit is used to prevent implicit constructor calls
        :value(value){};
};
class Literal_INT : public Expression {
public:
    int32_t value;

    void accept(Visitor& visitor) override;
    explicit Literal_INT(int32_t value)
        :value(value){};
};
class VariableReference : public Expression {
public:
    std::string varName;

    void accept(Visitor& visitor) override;
    explicit VariableReference(std::string varName)
        :varName(std::move(varName)){};
};
class Call : public Expression {
public:
    std::string functionName;
    std::vector<std::unique_ptr<Expression>> arguments;

    void accept(Visitor& visitor) override;
    Call(std::string name, std::vector<std::unique_ptr<Expression>> arguments)
        :functionName(std::move(name)), arguments(std::move(arguments)){};
};


//The below statement classes own children through std::unique_ptr (Block,
//VarDeclarationStatement, ...). A unique_ptr member only needs its type to be
//declared (forward declaration), but its destructor calls delete, which
//needs the type to be complete (fully defined) at the point where the
//destructor is generated.
//
//Every Node has a virtual destructor, and the compiler generates it implicitly
//at the closing brace of each class. If Block is still only forward-declared
//there, the build fails with "invalid application of 'sizeof' to incomplete
//type".
//
//unique_ptr<T> can hold an incomplete T, but destroying it can’t.
//
//Fix: define Block and VarDeclarationStatement BEFORE the classes that hold
//unique_ptrs to them (IfStatement, WhileStatement, ForStatement).
//
//Block only needs Statement to be complete (it holds unique_ptr<Statement>),
//not the concrete statements, so it can sit right below Statement.
//
//Alternative (not used): keep forward declarations and declare each
//destructor in the class, then define it `= default` in the .cpp where every
//type is complete.

//common type
class Statement : public Node {
};

class Block : public Node {
public:
    std::vector<std::unique_ptr<Statement>> statements;

    void accept(Visitor& visitor) override;
    Block() = default;
    explicit Block(std::vector<std::unique_ptr<Statement>> statements)
        :statements(std::move(statements)){};
};

class VarDeclarationStatement : public Statement {
public:
    VarType varType;
    std::string varName;
    std::unique_ptr<Expression> expression;

    void accept(Visitor& visitor) override;
    VarDeclarationStatement(VarType varType, std::string varName, std::unique_ptr<Expression> expression)
        :varType(varType), varName(std::move(varName)), expression(std::move(expression)){};
};

class IfStatement : public Statement {
public:
    IfStatementType ifStatementType;
    std::unique_ptr<Expression> ifCondition;
    std::unique_ptr<Block> ifBlock;
    std::unique_ptr<Block> elseBlock = nullptr; //regular IfStatements do not have else
    std::unique_ptr<IfStatement> elseIf = nullptr;

    void accept(Visitor& visitor) override;
    IfStatement(IfStatementType ifStatementType, std::unique_ptr<Expression> ifCondition, std::unique_ptr<Block> ifBlock, std::unique_ptr<Block> elseBlock, std::unique_ptr<IfStatement> elseIf)
        :ifStatementType(ifStatementType), ifCondition(std::move(ifCondition)), ifBlock(std::move(ifBlock)), elseBlock(std::move(elseBlock)), elseIf(std::move(elseIf)){};
};
class WhileStatement : public Statement {
public:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Block> block;

    void accept(Visitor &visitor) override;
    WhileStatement(std::unique_ptr<Expression> condition, std::unique_ptr<Block> block)
        :condition(std::move(condition)), block(std::move(block)){};

};
class ForStatement : public Statement {
public:
    std::unique_ptr<VarDeclarationStatement> forInit;
    std::unique_ptr<Expression> forCondition;
    std::unique_ptr<Expression> forIncrement;
    std::unique_ptr<Block> block;

    void accept(Visitor &visitor) override;
    ForStatement(std::unique_ptr<VarDeclarationStatement> forInit, std::unique_ptr<Expression> forCondition,std::unique_ptr<Expression> forIncrement, std::unique_ptr<Block> block)
        :forInit(std::move(forInit)), forCondition(std::move(forCondition)), forIncrement(std::move(forIncrement)), block(std::move(block)){};
};
class ReturnStatement : public Statement {
public:
    std::unique_ptr<Expression> returnExpression;

    void accept(Visitor& visitor) override;
    explicit ReturnStatement(std::unique_ptr<Expression> returnExpression)
        :returnExpression(std::move(returnExpression)){};
};
class ExpressStatement : public Statement {
public:
    std::unique_ptr<Expression> expression;

    void accept(Visitor& visitor) override;
    explicit ExpressStatement(std::unique_ptr<Expression> expression)
        :expression(std::move(expression)){};
};




class Function : public Node {
public:
    std::string functionName;
    VarType returnType;
    Block functionBlock;
    std::vector<Param> parameters;

    void accept(Visitor& visitor) override;
    Function(std::string functionName, Block functionBlock, std::vector<Param> parameters, VarType returnType)
        :functionName(std::move(functionName)), functionBlock(std::move(functionBlock)), parameters(std::move(parameters)), returnType(returnType){};
};

class Program : public Node {
public:
    std::vector<std::unique_ptr<Function>> functions;


    void accept(Visitor& visitor) override;
    explicit Program(std::vector<std::unique_ptr<Function>> functions)
        :functions(std::move(functions)){};
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
    uint32_t depth = 0;
    void printIndent();
    std::string getVarType(const VarType varType);
    std::string getUnaryOperator(const UnaryOperator unaryOp);
    std::string getBinaryOperator(const BinaryOperator binaryOp);


    void visit(Block& block) override;
    void visit(Function& function) override;
    void visit(Program& program) override;
    //statement
    void visit(IfStatement& ifStatement) override;
    void visit(ForStatement& forStatement) override;
    void visit(WhileStatement& whileStatement) override;
    void visit(ReturnStatement& returnStatement) override;
    void visit(ExpressStatement& expressStatement) override;
    void visit(VarDeclarationStatement& varDeclarationStatement) override;
    //expression
    void visit(UnaryExpression& unaryExpression) override;
    void visit(BinaryExpression& binaryExpression) override;
    void visit(AssignExpression& assignExpression) override;
    void visit(Literal_BOOL& literalBool) override;
    void visit(Literal_INT& literalInt) override;
    void visit(VariableReference& variableReference) override;
    void visit(Call& call) override;

    virtual ~DumpVisitor() = default;
};

#endif //NTR_TOOLCHAIN_NODE_H