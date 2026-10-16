//
// Created by tooka on 15/10/2026.
//

#include "Node.h"

void Literal_BOOL::accept(Visitor &visitor) {
    visitor.visit(*this); // *this is the dereferenced pointer of this instance
}

void Literal_INT::accept(Visitor &visitor) {
    visitor.visit(*this);
}

Literal_INT::Literal_INT(int value) {
    this->value = value;
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




