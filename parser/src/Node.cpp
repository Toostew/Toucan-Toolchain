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






void DumpVisitor::visit(Literal_BOOL& literal_bool) {
    printIndent();
    std::cout << "BOOL " <<(literal_bool.value ? "TRUE" : "FALSE") << "\n";
}

void DumpVisitor::visit(Literal_INT& literal_int) {
    printIndent();
    std::cout << "INT " << literal_int.value << "\n";
}

void DumpVisitor::visit(Block& block) {
    printIndent();
    std::cout << "BLOCK" << "\n";

    depth++;
    for (const std::unique_ptr<Statement>& statement : block.statements) {
        statement->accept(*this);
    }
    depth--;
}
void DumpVisitor::visit(Function& function) {
    printIndent();
    std::cout << "FUNCTION " << function.functionName << "(";
    for (int i = 0; i < function.parameters.size(); i++) {
        std::cout << getVarType(function.parameters[i].varType) << " ";
        std::cout << function.parameters[i].name;
        if (i < function.parameters.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << ")";
    std::cout << "(" << getVarType(function.returnType) << ")" << "\n";
    depth++;
    function.functionBlock.accept(*this);
    depth--;
}
void DumpVisitor::visit(Program& program) {
    printIndent();
    std::cout << "PROGRAM" << "\n";
    depth++;
    for (const std::unique_ptr<Function>& function : program.functions) {
        function->accept(*this);
    }
    depth--;
}

void DumpVisitor::visit(IfStatement& ifStatement) {
    printIndent();
    std::cout << "IF" << "\n";
    depth++;
    printIndent();
    std::cout << "COND" << "\n";
    depth++;
    ifStatement.ifCondition->accept(*this);
    depth--;
    printIndent();
    std::cout << "THEN" << "\n";
    depth++;
    ifStatement.ifBlock->accept(*this);
    depth--;
    switch (ifStatement.ifStatementType) {
        case IfStatementType::IF:
            break;
        case IfStatementType::IFELSE:
            printIndent();
            std::cout << "ELSE" << "\n";
            depth++;
            ifStatement.elseBlock->accept(*this);
            depth--;
            break;
        case IfStatementType::IFELSEIF:
            printIndent();
            std::cout << "ELSE" << "\n";
            depth++;
            ifStatement.elseIf->accept(*this);
            depth--;
            break;
    }
    depth--;
}
void DumpVisitor::visit(ForStatement& forStatement) {
    printIndent();
    std::cout << "FOR" << "\n";
    depth++;
    printIndent();
    std::cout << "INIT" << "\n";
    depth++;
    forStatement.forInit->accept(*this);
    depth--;
    printIndent();
    std::cout << "COND" << "\n";
    depth++;
    forStatement.forCondition->accept(*this);
    depth--;
    printIndent();
    std::cout << "INCR" << "\n";
    depth++;
    forStatement.forIncrement->accept(*this);
    depth--;
    forStatement.block->accept(*this);
    depth--;
}
void DumpVisitor::visit(WhileStatement& whileStatement) {
    printIndent();
    std::cout << "WHILE" << "\n";
    depth++;
    printIndent();
    std::cout << "COND" << "\n";
    depth++;
    whileStatement.condition->accept(*this);
    depth--;
    whileStatement.block->accept(*this);
    depth--;
}
void DumpVisitor::visit(ReturnStatement& returnStatement) {
    printIndent();
    std::cout << "RETURN" << "\n";
    depth++;
    if (returnStatement.returnExpression) {
        //is populated
        returnStatement.returnExpression->accept(*this);
    } else {
        //not; is a void return

    }
    depth--;
}
void DumpVisitor::visit(ExpressStatement& expressStatement) {
    printIndent();
    std::cout << "EXPRESS STATEMENT" << "\n";
    depth++;
    expressStatement.expression->accept(*this);
    depth--;
}
void DumpVisitor::visit(VarDeclarationStatement& varDeclarationStatement) {
    printIndent();
    std::cout << "VAR DECLARATION " << getVarType(varDeclarationStatement.varType) << " " << varDeclarationStatement.varName << "\n";
    depth++;
    varDeclarationStatement.expression->accept(*this);
    depth--;
}

void DumpVisitor::visit(UnaryExpression& unaryExpression) {
    printIndent();
    std::cout << getUnaryOperator(unaryExpression.op) << "\n";
    depth++;
    unaryExpression.operand->accept(*this);
    depth--;
}
void DumpVisitor::visit(BinaryExpression& binaryExpression) {
    printIndent();
    std::cout << getBinaryOperator(binaryExpression.op) << "\n";
    depth++;
    binaryExpression.leftOperand->accept(*this);
    binaryExpression.rightOperand->accept(*this);
    depth--;
}
void DumpVisitor::visit(AssignExpression& assignExpression) {
    printIndent();
    std::cout << "ASSIGN " << assignExpression.target << "\n";
    depth++;
    assignExpression.expression->accept(*this);
    depth--;
}
void DumpVisitor::visit(VariableReference& variableReference) {
    printIndent();
    std::cout << variableReference.varName << "\n";
}
void DumpVisitor::visit(Call& call) {
    printIndent();
    std::cout << call.functionName << "\n";
    depth++;
    std::cout << "ARGS" << "\n";
    depth ++;
    for (const std::unique_ptr<Expression>& arg : call.arguments) {
        arg->accept(*this);
    }
    depth--;
    depth--;
}


void DumpVisitor::printIndent() {

    for (int i = 0; i < depth; i++) {
        std::cout << "  "; //double space
    }
}

std::string DumpVisitor::getVarType(const VarType varType) {
    switch (varType) {
        case VarType::BOOL:
            return "BOOL";
        case VarType::INT:
            return "INT";
        case VarType::VOID:
            return "VOID";
        default:
            return "UNKNOWN";
    }
}

std::string DumpVisitor::getUnaryOperator(const UnaryOperator unaryOp) {
    switch (unaryOp) {
        case UnaryOperator::NEGATIVE:
            return "NEGATIVE";
        case UnaryOperator::NOT:
            return "NOT";
        default:
            return "UNKNOWN";
    }
}

std::string DumpVisitor::getBinaryOperator(const BinaryOperator binaryOp) {
    switch (binaryOp) {
        case BinaryOperator::ADD:
            return "ADD";
        case BinaryOperator::MINUS:
            return "MINUS";
        case BinaryOperator::MULTIPLY:
            return "MULTIPLY";
        case BinaryOperator::DIVIDE:
            return "DIVIDE";
        case BinaryOperator::AND:
            return "AND";
        case BinaryOperator::OR:
            return "OR";
        case BinaryOperator::EQUAL:
            return "EQUAL";
        case BinaryOperator::NOTEQUAL:
            return "NOTEQUAL";
        case BinaryOperator::MORETHAN:
            return "MORETHAN";
        case BinaryOperator::LESSTHAN:
            return "LESSTHAN";
        case BinaryOperator::MORETHANOREQUAL:
            return "MORETHANOREQUAL";
        case BinaryOperator::LESSTHANOREQUAL:
            return "LESSTHANOREQUAL";
        default:
            return "UNKNOWN";
    }
}








