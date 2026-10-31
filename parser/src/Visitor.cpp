//
// Created by tooka on 31/10/2026.
//

#include "Node.h"
#include "parser.h"
#include "Visitor.h"


//DumpVisitor

void DumpVisitor::visit(Literal_BOOL& literal_bool) {
    printIndent();
    std::cout << "BOOL " <<(literal_bool.value ? "TRUE" : "FALSE") << " ";
    printLines(literal_bool.lines);
}

void DumpVisitor::visit(Literal_INT& literal_int) {
    printIndent();
    std::cout << "INT " << literal_int.value << " ";
    printLines(literal_int.lines);
}

void DumpVisitor::visit(Block& block) {
    printIndent();
    std::cout << "BLOCK ";
    printLines(block.lines);

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
    std::cout << ") ";
    std::cout << "(" << getVarType(function.returnType) << ") ";
    printLines(function.lines);
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
    std::cout << "IF" << " ";
    printLines(ifStatement.lines);
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
    std::cout << "FOR" << " ";
    printLines(forStatement.lines);
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
    std::cout << "WHILE" << " ";
    printLines(whileStatement.lines);
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
    std::cout << "RETURN" << " ";
    printLines(returnStatement.lines);
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
    std::cout << "EXPRESS STATEMENT" << " ";
    printLines(expressStatement.lines);
    depth++;
    expressStatement.expression->accept(*this);
    depth--;
}
void DumpVisitor::visit(VarDeclarationStatement& varDeclarationStatement) {
    printIndent();
    std::cout << "VAR DECLARATION " << getVarType(varDeclarationStatement.varType) << " " << varDeclarationStatement.varName << " ";
    printLines(varDeclarationStatement.lines);
    depth++;
    varDeclarationStatement.expression->accept(*this);
    depth--;
}

void DumpVisitor::visit(UnaryExpression& unaryExpression) {
    printIndent();
    std::cout << "UNARY " << getUnaryOperator(unaryExpression.op) << " ";
    printLines(unaryExpression.lines);
    depth++;
    unaryExpression.operand->accept(*this);
    depth--;
}
void DumpVisitor::visit(BinaryExpression& binaryExpression) {
    printIndent();
    std::cout << "OP " <<getBinaryOperator(binaryExpression.op) << " ";
    printLines(binaryExpression.lines);
    depth++;
    binaryExpression.leftOperand->accept(*this);
    binaryExpression.rightOperand->accept(*this);
    depth--;
}
void DumpVisitor::visit(AssignExpression& assignExpression) {
    printIndent();
    std::cout << "ASSIGN " << assignExpression.target << " ";
    printLines(assignExpression.lines);
    depth++;
    assignExpression.expression->accept(*this);
    depth--;
}
void DumpVisitor::visit(VariableReference& variableReference) {
    printIndent();
    std::cout << "VAR " << variableReference.varName << " ";
    printLines(variableReference.lines);
}
void DumpVisitor::visit(Call& call) {
    printIndent();
    std::cout << "CALL " << call.functionName << " ";
    printLines(call.lines);
    depth++;
    printIndent();
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

void DumpVisitor::printLines(std::pair<size_t,size_t> lines) {
    if (lines.first == lines.second) {
        //single line
        std::cout << "[" << lines.first << "]"<<"\n";
    } else {
        //multiple lines
        std::cout << "[" << lines.first << "," << lines.second << "]"<<"\n";
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
        case BinaryOperator::PLUS:
            return "PLUS";
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




//SemanticVisitor


//check Program has atleast one function called main with no params, get all functions into a central table
void SemanticVisitor::functionSignatureCheck(Program& program) {

    //populate list
    for (const auto& function : program.functions) {
        functionMD tempMD = {function->parameters, function->returnType};
        if (functionTable.find(function->functionName) != functionTable.end()) {

        }
        functionTable[function->functionName] = tempMD;
    }

    //check list
    int mainCount = 0;
    for (const auto& function : functionTable) {
        if (function.first == "main") {

        }
    }


}

