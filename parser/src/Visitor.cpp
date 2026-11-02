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



std::string getVarType(const VarType varType) {
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

std::string getUnaryOperator(const UnaryOperator unaryOp) {
    switch (unaryOp) {
        case UnaryOperator::NEGATIVE:
            return "NEGATIVE";
        case UnaryOperator::NOT:
            return "NOT";
        default:
            return "UNKNOWN";
    }
}

std::string getBinaryOperator(const BinaryOperator binaryOp) {
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
void SemanticVisitor::functionSignatureCheck(const Program& program) {

    //populate list
    for (const auto& function : program.functions) {
        functionMD tempMD = {function->parameters, function->returnType};
        if (functionTable.find(function->functionName) == functionTable.end()) {
            //functionTable doesn't have this entry
            functionTable[function->functionName] = tempMD;
        } else {
            //duplicate function names
            std::string message = "Multiple functions with the same name " + function->functionName + " not allowed";
            errors.push_back(error{message,function->lines});
        }

    }

    //check list for main with no params
    int mainCount = 0;
    VarType returnType = VarType::UNKNOWN;
    for (const auto& function : functionTable) {
        if (function.first == "main") {
            if (function.second.parameters.empty()) {
                //no params no problem
                mainCount++;
                returnType = function.second.returnType;
            } else {
                std::string message = "main function must be parameterless";
                errors.push_back(error{message,std::pair<size_t,size_t>{0,0}});
            }
        }
    }
    if (mainCount == 0) {
        errors.push_back(error{"no parameterless main function", std::pair<size_t,size_t>{0,0}});
    }

    //check returnType
    if (returnType != VarType::INT && mainCount == 1) {
        errors.push_back(error{"returnType of main function must be int", std::pair<size_t,size_t>{0,0}});
    }

}



void SemanticVisitor::functionBodyCheck(const Program &program) {

}


void SemanticVisitor::visit(Program &program) {

    for (const auto& function : program.functions) {
        function->accept(*this);
    }

}


void SemanticVisitor::visit(Function &function) {
    //return type must match!


    std::unordered_map<std::string, variableMD> variableTable;
    for (const auto& param : function.parameters) {
        auto var = variableTable.find(param.name);
        if (var != variableTable.end()) {
            //match, shouldnt
            std::string message = "parameter " + param.name + " in " + function.functionName + " is already defined";
            errors.push_back(error{message,std::pair<size_t,size_t>{function.lines.first,function.lines.first}});
        }
        variableTable[param.name] = variableMD{param.varType,std::pair<size_t,size_t>{function.lines.first,function.lines.first}}; //duplicate params overwrite, doesnt matter
    }
    variableTableStack.push_back(variableTable);
    currFunctionReturnType = function.returnType;

    function.functionBlock.accept(*this);
    if (function.returnType != VarType::VOID && !hasReturn) {
        std::string message = "function " + function.functionName + " does not have sufficient returns of type " + getVarType(function.returnType);
        errors.push_back(error{message,function.lines});
    }

    variableTableStack.pop_back();
    currFunctionReturnType = VarType::UNKNOWN;
}

void SemanticVisitor::visit(Block& block) {
    std::unordered_map<std::string, variableMD> variableTable;
    variableTableStack.push_back(variableTable);

    //detect the first occurrence of a return statement
    //every statement AFTER that is non-reachable (warning)
    bool localHasReturn = false;
    bool hasWarned = false;
    std::pair<size_t,size_t> lines = block.lines;
    for (const auto& statement : block.statements) {
        if (localHasReturn && !hasWarned) {
            hasWarned = true;
            lines.first = statement->lines.first;
        }
        statement->accept(*this);
        if (hasReturn) {
            localHasReturn = true;
        }
    }
    if (localHasReturn && hasWarned) {
        warnings.push_back(error{"Code unreachable",lines});
    }

    variableTableStack.pop_back();
    hasReturn = localHasReturn;
}

void SemanticVisitor::visit(IfStatement &ifStatement) {

    ifStatement.ifCondition->accept(*this);
    //we detect anything that isnt bool and unknown. unknown will already get an error reported anyway
    if (ifStatement.ifCondition->varType != VarType::BOOL && ifStatement.ifCondition->varType != VarType::UNKNOWN) {
        errors.push_back(error{"If condition requires boolean expression",ifStatement.lines});
    }

    bool localIfHasReturn = false;
    bool localElseHasReturn = false;
    if (ifStatement.ifStatementType == IfStatementType::IF) {
        //if then
        ifStatement.ifBlock->accept(*this);
        localIfHasReturn = false;
    }
    else {
        //if else if, if else
        ifStatement.ifBlock->accept(*this);
        localIfHasReturn = hasReturn;
        ifStatement.elseBlock == nullptr ? ifStatement.elseIf->accept(*this) : ifStatement.elseBlock->accept(*this);
        localElseHasReturn = hasReturn;

        localIfHasReturn = localIfHasReturn && localElseHasReturn;
    }
    hasReturn = localIfHasReturn;
}

void SemanticVisitor::visit(WhileStatement &whileStatement) {
    whileStatement.condition->accept(*this);

    if (whileStatement.condition->varType != VarType::BOOL && whileStatement.condition->varType != VarType::UNKNOWN) {
        errors.push_back(error{"while condition requires boolean expression",whileStatement.lines});
    }

    whileStatement.block->accept(*this);
    hasReturn = false;
}

void SemanticVisitor::visit(ForStatement &forStatement) {
    std::unordered_map<std::string, variableMD> variableTable;
    variableTableStack.push_back(variableTable);
    forStatement.forInit->accept(*this);
    forStatement.forCondition->accept(*this);
    if (forStatement.forCondition->varType != VarType::BOOL && forStatement.forCondition->varType != VarType::UNKNOWN) {
        errors.push_back(error{"For condition requires boolean expression",forStatement.lines});
    }
    forStatement.forIncrement->accept(*this);
    forStatement.block->accept(*this);
    variableTableStack.pop_back();
    hasReturn = false;
}

void SemanticVisitor::visit(ReturnStatement &returnStatement) {
    if (returnStatement.returnExpression != nullptr) {
        returnStatement.returnExpression->accept(*this);
        if (returnStatement.returnExpression->varType != currFunctionReturnType && returnStatement.returnExpression->varType != VarType::UNKNOWN) {
            errors.push_back(error{"Invalid return type, expected " + getVarType(currFunctionReturnType) + ", got " + getVarType(returnStatement.returnExpression->varType),returnStatement.lines});

        }
    } else {
        //no returnExpression, it is void return, anything that isnt void and isnt unknown
        if (currFunctionReturnType != VarType::VOID && currFunctionReturnType != VarType::UNKNOWN) {
            errors.push_back(error{"Invalid return type, expected " + getVarType(currFunctionReturnType) + ", got void",returnStatement.lines});
        }
    }
    hasReturn = true;
}

void SemanticVisitor::visit(ExpressStatement &expressStatement) {
    expressStatement.expression->accept(*this);
    hasReturn = false;
}

void SemanticVisitor::visit(VarDeclarationStatement &varDeclarationStatement) {
    varDeclarationStatement.expression->accept(*this);
    if (varDeclarationStatement.varType != varDeclarationStatement.expression->varType && varDeclarationStatement.expression->varType != VarType::UNKNOWN) {
        //var declaration does not match the type produced by the expression
        std::string message = "var " + varDeclarationStatement.varName + " expected an expression of type " + getVarType(varDeclarationStatement.varType) + ", got " + getVarType(varDeclarationStatement.expression->varType);
        errors.push_back(error{message,varDeclarationStatement.lines});
    }
    std::pair<std::string, variableMD> var = findVariable(varDeclarationStatement.varName);
    if (var.first != "" && var.second.varType != VarType::UNKNOWN) {
        errors.push_back(error{"var " + var.first + " already defined",var.second.lines});
    } else {
        //no duplicates
        variableTableStack.back()[varDeclarationStatement.varName] = variableMD{varDeclarationStatement.varType,varDeclarationStatement.lines};
    }
    hasReturn = false;
}

void SemanticVisitor::visit(AssignExpression &assignExpression) {

}





std::pair<std::string, variableMD> SemanticVisitor::findVariable(const std::string& varName) {

    for (const auto& variableTable : variableTableStack) {
        auto var = variableTable.find(varName);
        if (var != variableTable.end()) {
            return std::pair<std::string, variableMD>({var->first,var->second});
        }
    }
    //could not find a copy
    return std::pair<std::string, variableMD>({"",variableMD{VarType::UNKNOWN, std::pair<size_t,size_t>(0,0)}});
}





