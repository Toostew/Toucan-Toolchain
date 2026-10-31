//
// Created by tooka on 15/10/2026.
//

#include "Node.h"
#include "Visitor.h"

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
void Call::accept(Visitor &visitor) {
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














