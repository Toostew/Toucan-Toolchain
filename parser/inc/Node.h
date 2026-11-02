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
    std::pair<size_t, size_t> lines;
    Node(std::pair<size_t, size_t> line)
        : lines(line){};
    virtual ~Node() {};
    virtual void accept(Visitor& visitor) = 0; //pure virtual

};



//common type
class Expression : public Node {
public:
    VarType varType = VarType::UNKNOWN; //init default to unknown

    Expression(std::pair<size_t, size_t>  line)
        : Node(line){};
};
class UnaryExpression : public Expression{
public:
    UnaryOperator op;
    std::unique_ptr<Expression> operand;

    void accept(Visitor& visitor) override;
    UnaryExpression(UnaryOperator op, std::unique_ptr<Expression> operand, std::pair<size_t, size_t>  line)
        :op(op), operand(std::move(operand)), Expression(line){};
};
class BinaryExpression : public Expression {
public:
    BinaryOperator op;
    std::unique_ptr<Expression> leftOperand;
    std::unique_ptr<Expression> rightOperand;

    void accept(Visitor& visitor) override;
    BinaryExpression(BinaryOperator op, std::unique_ptr<Expression> leftOperand, std::unique_ptr<Expression> rightOperand, std::pair<size_t, size_t>  line)
        :op(op), leftOperand(std::move(leftOperand)), rightOperand(std::move(rightOperand)), Expression(line){};
};
class AssignExpression : public Expression {
public:
    std::string target;
    std::unique_ptr<Expression> expression;

    void accept(Visitor& visitor) override;
    AssignExpression(std::string target, std::unique_ptr<Expression> expression, std::pair<size_t, size_t>  line)
        :target(std::move(target)), expression(std::move(expression)), Expression(line){};
};
class Literal_BOOL : public Expression {
public:
    bool value;

    void accept(Visitor& visitor) override;
    explicit Literal_BOOL(bool value, std::pair<size_t, size_t>  line) //explicit is used to prevent implicit constructor calls
        :Expression(line), value(value) {
        this->varType = VarType::BOOL; //bools vartypes are absolute
    };
};
class Literal_INT : public Expression {
public:
    int32_t value;

    void accept(Visitor& visitor) override;
    explicit Literal_INT(int32_t value, std::pair<size_t, size_t>  line)
        :value(value), Expression(line) {
        this->varType = VarType::INT; //int vartype is absolute
    };
};
class VariableReference : public Expression {
public:
    std::string varName;

    void accept(Visitor& visitor) override;
    explicit VariableReference(std::string varName, std::pair<size_t, size_t>  line)
        :varName(std::move(varName)), Expression(line){};
};
class Call : public Expression {
public:
    std::string functionName;
    std::vector<std::unique_ptr<Expression>> arguments;

    void accept(Visitor& visitor) override;
    Call(std::string name, std::vector<std::unique_ptr<Expression>> arguments, std::pair<size_t, size_t>  line)
        :functionName(std::move(name)), arguments(std::move(arguments)), Expression(line){};
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
public:
    Statement(std::pair<size_t, size_t>  line)
        :Node(line){};
};

class Block : public Node {
public:
    std::vector<std::unique_ptr<Statement>> statements;

    void accept(Visitor& visitor) override;
    Block() = default;
    explicit Block(std::vector<std::unique_ptr<Statement>> statements, std::pair<size_t, size_t>  line)
        :statements(std::move(statements)), Node(line){};
};

class VarDeclarationStatement : public Statement {
public:
    VarType varType;
    std::string varName;
    std::unique_ptr<Expression> expression;

    void accept(Visitor& visitor) override;
    VarDeclarationStatement(VarType varType, std::string varName, std::unique_ptr<Expression> expression, std::pair<size_t, size_t>  line)
        :varType(varType), varName(std::move(varName)), expression(std::move(expression)), Statement(line){};
};

class IfStatement : public Statement {
public:
    IfStatementType ifStatementType;
    std::unique_ptr<Expression> ifCondition;
    std::unique_ptr<Block> ifBlock;
    std::unique_ptr<Block> elseBlock = nullptr; //regular IfStatements do not have else
    std::unique_ptr<IfStatement> elseIf = nullptr;

    void accept(Visitor& visitor) override;
    IfStatement(IfStatementType ifStatementType, std::unique_ptr<Expression> ifCondition, std::unique_ptr<Block> ifBlock, std::unique_ptr<Block> elseBlock, std::unique_ptr<IfStatement> elseIf, std::pair<size_t, size_t>  line)
        :ifStatementType(ifStatementType), ifCondition(std::move(ifCondition)), ifBlock(std::move(ifBlock)), elseBlock(std::move(elseBlock)), elseIf(std::move(elseIf)), Statement(line){};
};
class WhileStatement : public Statement {
public:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Block> block;

    void accept(Visitor &visitor) override;
    WhileStatement(std::unique_ptr<Expression> condition, std::unique_ptr<Block> block, std::pair<size_t, size_t>  line)
        :condition(std::move(condition)), block(std::move(block)), Statement(line){};

};
class ForStatement : public Statement {
public:
    std::unique_ptr<VarDeclarationStatement> forInit;
    std::unique_ptr<Expression> forCondition;
    std::unique_ptr<Expression> forIncrement;
    std::unique_ptr<Block> block;

    void accept(Visitor &visitor) override;
    ForStatement(std::unique_ptr<VarDeclarationStatement> forInit, std::unique_ptr<Expression> forCondition,std::unique_ptr<Expression> forIncrement, std::unique_ptr<Block> block, std::pair<size_t, size_t>  line)
        :forInit(std::move(forInit)), forCondition(std::move(forCondition)), forIncrement(std::move(forIncrement)), block(std::move(block)), Statement(line){};
};
class ReturnStatement : public Statement {
public:
    std::unique_ptr<Expression> returnExpression;

    void accept(Visitor& visitor) override;
    explicit ReturnStatement(std::unique_ptr<Expression> returnExpression, std::pair<size_t, size_t>  line)
        :returnExpression(std::move(returnExpression)), Statement(line){};
};
class ExpressStatement : public Statement {
public:
    std::unique_ptr<Expression> expression;

    void accept(Visitor& visitor) override;
    explicit ExpressStatement(std::unique_ptr<Expression> expression, std::pair<size_t, size_t>  line)
        :expression(std::move(expression)), Statement(line){};
};




class Function : public Node {
public:
    std::string functionName;
    VarType returnType;
    Block functionBlock;
    std::vector<Param> parameters;

    void accept(Visitor& visitor) override;
    Function(std::string functionName, Block functionBlock, std::vector<Param> parameters, VarType returnType, std::pair<size_t, size_t>  line)
        :functionName(std::move(functionName)), functionBlock(std::move(functionBlock)), parameters(std::move(parameters)), returnType(returnType), Node(line){};
};

class Program : public Node {
public:
    std::vector<std::unique_ptr<Function>> functions;


    void accept(Visitor& visitor) override;
    explicit Program(std::vector<std::unique_ptr<Function>> functions, std::pair<size_t, size_t>  line)
        :functions(std::move(functions)), Node(line){};
};



#endif //NTR_TOOLCHAIN_NODE_H