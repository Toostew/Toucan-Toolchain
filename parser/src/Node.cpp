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






void dumpVisitor::visit(Literal_BOOL& literal_bool) {
    std::cout << literal_bool.value << std::endl;
}

void dumpVisitor::visit(Literal_INT& literal_int) {
    std::cout << literal_int.value << std::endl;
}

void dumpVisitor::visit(Block& block) {

}
void dumpVisitor::visit(Function& function){}
void dumpVisitor::visit(Program& program){}

void dumpVisitor::visit(IfStatement& ifStatement){}
void dumpVisitor::visit(ForStatement& forStatement){}
void dumpVisitor::visit(WhileStatement& whileStatement){}
void dumpVisitor::visit(ReturnStatement& returnStatement){}
void dumpVisitor::visit(ExpressStatement& expressStatement){}
void dumpVisitor::visit(VarDeclarationStatement& varDeclarationStatement){}

void dumpVisitor::visit(UnaryExpression& unaryExpression){}
void dumpVisitor::visit(BinaryExpression& binaryExpression){}
void dumpVisitor::visit(AssignExpression& assignExpression){}
void dumpVisitor::visit(VarReference& varReference){}
void dumpVisitor::visit(Call& call){}




