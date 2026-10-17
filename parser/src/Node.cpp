//
// Created by tooka on 15/10/2026.
//

#include "Node.h"

void UnaryExpression::accept(Visitor &visitor) {
    visitor.visit(*this);
}
void BinaryExpression::accept(Visitor &visitor) {
    visitor.visit(*this);
}
void AssignExpression::accept(Visitor &visitor) {
    visitor.visit(*this);
}
void Literal_BOOL::accept(Visitor &visitor) {
    visitor.visit(*this);
}
void Literal_INT::accept(Visitor &visitor) {
    visitor.visit(*this);
}
void VariableReference::accept(Visitor &visitor) {
    visitor.visit(*this);
}







void Block::accept(Visitor &visitor) {
    visitor.visit(*this);
}
void VarDeclarationStatement::accept(Visitor &visitor) {
    visitor.visit(*this);
}
void IfStatement::accept(Visitor &visitor) {
    visitor.visit(*this);
}
void WhileStatement::accept(Visitor &visitor) {
    visitor.visit(*this);
}
void ForStatement::accept(Visitor &visitor) {
    visitor.visit(*this);
}
void ReturnStatement::accept(Visitor &visitor) {
    visitor.visit(*this);
}
void ExpressStatement::accept(Visitor &visitor) {
    visitor.visit(*this);
}
void Function::accept(Visitor &visitor) {
    visitor.visit(*this);
}
void Program::accept(Visitor &visitor) {
    visitor.visit(*this);
}







































void DumpVisitor::visit(Literal_BOOL& literal_bool) {
    std::cout << literal_bool.value << std::endl;
}

void DumpVisitor::visit(Literal_INT& literal_int) {
    std::cout << literal_int.value << std::endl;
}

void DumpVisitor::visit(Block& block) {

}
void DumpVisitor::visit(Function& function){}
void DumpVisitor::visit(Program& program){}

void DumpVisitor::visit(IfStatement& ifStatement){}
void DumpVisitor::visit(ForStatement& forStatement){}
void DumpVisitor::visit(WhileStatement& whileStatement){}
void DumpVisitor::visit(ReturnStatement& returnStatement){}
void DumpVisitor::visit(ExpressStatement& expressStatement){}
void DumpVisitor::visit(VarDeclarationStatement& varDeclarationStatement){}

void DumpVisitor::visit(UnaryExpression& unaryExpression){}
void DumpVisitor::visit(BinaryExpression& binaryExpression){}
void DumpVisitor::visit(AssignExpression& assignExpression){}
void DumpVisitor::visit(VariableReference& variableReference){}
void DumpVisitor::visit(Call& call){}




