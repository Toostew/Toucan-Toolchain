//
// Created by tooka on 31/10/2026.
//

#ifndef NTR_TOOLCHAIN_VISITOR_H
#define NTR_TOOLCHAIN_VISITOR_H

#include "Node.h"


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



#endif //NTR_TOOLCHAIN_VISITOR_H