#include "sema/SemanticAnalyzer.hpp"
#include "visitor/AstNodeInclude.hpp"

void SemanticAnalyzer::visit(ProgramNode &p_program) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    symbol_manager.pushScope(new SymbolTable(p_program.get_return_type()));
    symbol_manager.pushSymbol(p_program.getNameCString(), "program", symbol_manager.get_level(), p_program.getType(), "");
    p_program.visitChildNodes(*this);
    symbol_manager.printCurrentScope();
    symbol_manager.popScope();
    if (error_message.size() > 0) {
        for (auto &message : error_message) {
            printf("%s\n", message.c_str());
        }
        exit(1);
    }
}

void SemanticAnalyzer::visit(DeclNode &p_decl) {
    p_decl.visitChildNodes(*this);
}

void SemanticAnalyzer::visit(VariableNode &p_variable) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */

    // check if loop variable is redeclared
    for (auto &var_node : loop_variable) {
        if (var_node == p_variable.getNameCString()) {
            dump_error(p_variable.getLocation().line, p_variable.getLocation().col, "symbol '" + std::string(p_variable.getNameCString()) + "' is redeclared");
            p_variable.faulty = true;
            return;
        }
    }
    //check if variable is redeclared in same scope
    if (symbol_manager.lookup(p_variable.getNameCString())) {
        dump_error(p_variable.getLocation().line, p_variable.getLocation().col, "symbol '" + std::string(p_variable.getNameCString()) + "' is redeclared");
        p_variable.faulty = true;
    }
    else {
        symbol_manager.pushSymbol(p_variable.getNameCString(), p_variable.getKind(), symbol_manager.get_level(), p_variable.getType(), p_variable.getAttribute());
    }
    //check if variable is declared as an array with an index that is not greater than 0
    std::vector<uint64_t> dim = p_variable.getType()->getDimensions();
    for (auto i : dim) {
        if (i <= 0) {
            dump_error(p_variable.getLocation().line, p_variable.getLocation().col, "'" + std::string(p_variable.getNameCString()) + "' declared as an array with an index that is not greater than 0");
            break;
        }
    }
    //append loop variable to loop variable redeclaration checking queue
    if (strcmp(p_variable.getKind(), "loop_var") == 0) {
        loop_variable.push_back(p_variable.getNameCString());
        loop_variable_level++;
    }
    p_variable.visitChildNodes(*this);
}

void SemanticAnalyzer::visit(ConstantValueNode &p_constant_value) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_constant_value.set_type(p_constant_value.getTypeSharedPtr());
    return;
    }

void SemanticAnalyzer::visit(FunctionNode &p_function) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    if (symbol_manager.lookup(p_function.getNameCString())) {
        dump_error(p_function.getLocation().line, p_function.getLocation().col, "symbol '" + std::string(p_function.getNameCString()) + "' is redeclared");
    }
    else {
        symbol_manager.pushSymbol(p_function.getNameCString(), "function", symbol_manager.get_level(), p_function.getType(), p_function.getAttribute());
    }
    symbol_manager.pushScope(new SymbolTable(p_function.get_return_type()));
    p_function.visitChildNodes(*this);
    symbol_manager.printCurrentScope();
    symbol_manager.popScope();
    }

void SemanticAnalyzer::visit(CompoundStatementNode &p_compound_statement) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    if (!p_compound_statement.get_is_funciton()) {
        symbol_manager.pushScope(new SymbolTable(symbol_manager.get_top_return_type()));
    }
    p_compound_statement.visitChildNodes(*this);
    if (!p_compound_statement.get_is_funciton()) {
        symbol_manager.printCurrentScope();
        symbol_manager.popScope();
    }
}

void SemanticAnalyzer::visit(PrintNode &p_print) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */

    p_print.visitChildNodes(*this);
    if (p_print.checkExpressionValid()) {
        if (!p_print.checkScalarType()) {
            dump_error(p_print.getChildLocation().line, p_print.getChildLocation().col, "expression of print statement must be scalar type");
        }
    }
}

void SemanticAnalyzer::visit(BinaryOperatorNode &p_bin_op) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_bin_op.visitChildNodes(*this);
    if (p_bin_op.validateChildNode()) {
        if (!p_bin_op.validateType()) {
            dump_error(p_bin_op.getLocation().line, p_bin_op.getLocation().col, "invalid operands to binary operator '" + std::string(p_bin_op.getOpCString()) + "' ('" + p_bin_op.getLeftOperand()->get_type()->getPTypeCString() + "' and '" + p_bin_op.getRightOperand()->get_type()->getPTypeCString() + "')");
        }
    }
}

void SemanticAnalyzer::visit(UnaryOperatorNode &p_un_op) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_un_op.visitChildNodes(*this);
    if (p_un_op.validateChildNode()) {
        if (!p_un_op.validateType()) {
            dump_error(p_un_op.getLocation().line, p_un_op.getLocation().col, "invalid operand to unary operator '" + std::string(p_un_op.getOpCString()) + "' ('" + p_un_op.getOperand()->get_type()->getPTypeCString() + "')");
        }
    }
}

void SemanticAnalyzer::visit(FunctionInvocationNode &p_func_invocation) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_func_invocation.visitChildNodes(*this);
    SymbolEntry* entry = symbol_manager.lookup_global(p_func_invocation.getNameCString());
    if (entry == nullptr) {
        dump_error(p_func_invocation.getLocation().line, p_func_invocation.getLocation().col, "use of undeclared symbol '" + std::string(p_func_invocation.getNameCString()) + "'");
        return;
    }
    if (strcmp(entry->kind, "function") != 0) {
        dump_error(p_func_invocation.getLocation().line, p_func_invocation.getLocation().col, "call of non-function symbol '" + std::string(p_func_invocation.getNameCString()) + "'");
        return;
    }
    std::vector<std::string> parameter;
    const char* attribute = entry->attribute;
    std::string temp = "";
    for (int i=0; i<strlen(attribute); i++) {
        if (attribute[i] == ',') {
            parameter.push_back(temp);
            temp = "";
            i++;
        }
        else {
            temp += attribute[i];
        }
    }
    if (temp != "") {
        parameter.push_back(temp);
    }
    if (p_func_invocation.checkArgNum(parameter.size())) {
        std::vector<ExpressionNode*> args = p_func_invocation.getArgs();
        for (int i=0; i<static_cast<int>(args.size()); ++i) {
            if (args[i]->get_type() == nullptr) return;
            if (strcmp(args[i]->get_type()->getPTypeCString(), parameter[i].c_str()) != 0) {
                if (strcmp(args[i]->get_type()->getPTypeCString(), "integer") == 0 && parameter[i] == "real") continue;
                dump_error(args[i]->getLocation().line, args[i]->getLocation().col, "incompatible type passing '" + std::string(args[i]->get_type()->getPTypeCString()) + "' to parameter of type '" + parameter[i] + "'");
                return;
            }
        }
    }
    else {
        dump_error(p_func_invocation.getLocation().line, p_func_invocation.getLocation().col, "too few/much arguments provided for function '" + std::string(p_func_invocation.getNameCString()) + "'");
        return;
    }
    PType ref = *entry->type;
    std::shared_ptr<PType> type = std::make_shared<PType>(ref);
    p_func_invocation.set_type(type);
    return;
}

void SemanticAnalyzer::visit(VariableReferenceNode &p_variable_ref) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_variable_ref.visitChildNodes(*this);
    SymbolEntry* entry = symbol_manager.lookup_global(p_variable_ref.getNameCString());
    if (entry == nullptr) {
        dump_error(p_variable_ref.getLocation().line, p_variable_ref.getLocation().col, "use of undeclared symbol '" + std::string(p_variable_ref.getNameCString()) + "'");
        return;
    }
    else if (strcmp(entry->kind, "function") == 0 || strcmp(entry->kind, "program") == 0) {
        dump_error(p_variable_ref.getLocation().line, p_variable_ref.getLocation().col, "use of non-variable symbol '" + std::string(p_variable_ref.getNameCString()) + "'");
        return;
    }
    if (!p_variable_ref.checkIndiceValid()) return;
    std::vector<const char*> dim = p_variable_ref.getDimensions();
    // check if this array in symbol table is valid
    if (entry->type->getDimensions().size() < dim.size()) {
        dump_error(p_variable_ref.getLocation().line, p_variable_ref.getLocation().col, "there is an over array subscript on '"+ std::string(p_variable_ref.getNameCString()) +"'");
        return;
    }
    for (int i = 0; i<static_cast<int>(dim.size()); ++i) {
        if (entry->type->getDimensions()[i] <= 0) return;
        const char* dim_str = dim[i];
        for (int j = 0; j < static_cast<int>(strlen(dim_str)); ++j) {
            if (!isdigit(dim_str[j]) && j > 0) {
                dump_error(p_variable_ref.get_child_location(i).line, p_variable_ref.get_child_location(i).col, "index of array reference must be an integer");
                return;
            }
            else if (j == 0 && dim_str[j] != '-' && !isdigit(dim_str[j])) {
                dump_error(p_variable_ref.get_child_location(i).line, p_variable_ref.get_child_location(i).col, "index of array reference must be an integer");
                return;
            }
        }
    }

    // pass PType to expression node
    PType ref = *entry->type;
    std::shared_ptr<PType> type = std::make_shared<PType>(ref);
    type->pop_dimension(dim.size());
    p_variable_ref.set_type(type);
    return;
}

void SemanticAnalyzer::visit(AssignmentNode &p_assignment) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_assignment.visitChildNodes(*this);
    VariableReferenceNode* var = p_assignment.getLvalue();
    ExpressionNode* expr = p_assignment.getExpr();
    if (var->get_type() == nullptr) return;
    if (var->get_type()->getDimensions().size() > 0) {
        dump_error(var->getLocation().line, var->getLocation().col, "array assignment is not allowed");
        return;
    }
    SymbolEntry* entry = symbol_manager.lookup_global(var->getNameCString());
    if (strcmp(entry->kind, "constant") == 0) {
        dump_error(var->getLocation().line, var->getLocation().col, "cannot assign to variable '" + std::string(var->getNameCString()) + "' which is a constant");
        return;
    }
    if (entry->level < symbol_manager.get_level() && strcmp(entry->kind, "loop_var") == 0) {
        dump_error(var->getLocation().line, var->getLocation().col, "the value of loop variable cannot be modified inside the loop body");
        return;
    }
    if (expr->get_type() == nullptr) return;
    if (expr->get_type()->getDimensions().size() > 0) {
        dump_error(expr->getLocation().line, expr->getLocation().col, "array assignment is not allowed");
        return;
    }
    if (strcmp(entry->type->getPTypeCString(), expr->get_type()->getPTypeCString()) != 0) {
        if (strcmp(entry->type->getPTypeCString(), "integer") == 0 && strcmp(expr->get_type()->getPTypeCString(), "real") == 0) return;
        else if (strcmp(expr->get_type()->getPTypeCString(), "integer") == 0 && strcmp(entry->type->getPTypeCString(), "real") == 0) return;
        dump_error(p_assignment.getLocation().line, p_assignment.getLocation().col, "assigning to '" + std::string(var->get_type()->getPTypeCString()) + "' from incompatible type '" + std::string(expr->get_type()->getPTypeCString()) + "'");
        return;
    }
}

void SemanticAnalyzer::visit(ReadNode &p_read) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_read.visitChildNodes(*this);
    VariableReferenceNode* var = p_read.getTarget();
    if (var->get_type() == nullptr) return;
    SymbolEntry* entry = symbol_manager.lookup_global(var->getNameCString());
    PType* type = entry->type;
    if (!type->isScalarType()) {
        dump_error(var->getLocation().line, var->getLocation().col, "variable reference of read statement must be scalar type");
    }
    if (strcmp(entry->kind, "constant") == 0 || strcmp(entry->kind, "loop_var") == 0) {
        dump_error(var->getLocation().line, var->getLocation().col, "variable reference of read statement cannot be a constant or loop variable");
    }
}

void SemanticAnalyzer::visit(IfNode &p_if) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_if.visitChildNodes(*this);
    ExpressionNode* expr = p_if.getCondition();
    if (expr->get_type() == nullptr) return;
    if (expr->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kBoolType) {
        dump_error(expr->getLocation().line, expr->getLocation().col, "the expression of condition must be boolean type");
    }
}

void SemanticAnalyzer::visit(WhileNode &p_while) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_while.visitChildNodes(*this);
    ExpressionNode* expr = p_while.getCondition();
    if (expr->get_type() == nullptr) return;
    if (expr->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kBoolType) {
        dump_error(expr->getLocation().line, expr->getLocation().col, "the expression of condition must be boolean type");
    }
}

void SemanticAnalyzer::visit(ForNode &p_for) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    int current_level = loop_variable_level;
    symbol_manager.pushScope(new SymbolTable(symbol_manager.get_top_return_type()));
    p_for.visitChildNodes(*this);
    if (!p_for.variableBoundCheck()) {
        dump_error(p_for.getLocation().line, p_for.getLocation().col, "the lower bound and upper bound of iteration count must be in the incremental order");
    }
    symbol_manager.printCurrentScope();
    symbol_manager.popScope();
    for (int i=0; i<loop_variable_level-current_level; ++i){
        loop_variable.pop_back();
    }
    loop_variable_level = current_level;
}

void SemanticAnalyzer::visit(ReturnNode &p_return) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_return.visitChildNodes(*this);
    ExpressionNode* expr = p_return.get_return_value();
    if (symbol_manager.get_top_return_type()->getPrimitiveType() == PType::PrimitiveTypeEnum::kVoidType) {
        dump_error(p_return.getLocation().line, p_return.getLocation().col, "program/procedure should not return a value");
    }
    else {
        if (expr->get_type() == nullptr) {
            return;
        }
        else {
            if (strcmp(symbol_manager.get_top_return_type()->getPTypeCString(), expr->get_type()->getPTypeCString()) != 0) {
                if (strcmp(symbol_manager.get_top_return_type()->getPTypeCString(), "integer") == 0 && strcmp(expr->get_type()->getPTypeCString(), "real") == 0) return;
                else if (strcmp(expr->get_type()->getPTypeCString(), "integer") == 0 && strcmp(symbol_manager.get_top_return_type()->getPTypeCString(), "real") == 0) return;
                dump_error(p_return.getChildLocation().line, p_return.getChildLocation().col, "return '" + std::string(expr->get_type()->getPTypeCString()) + "' from a function with return type '" + std::string(symbol_manager.get_top_return_type()->getPTypeCString()) + "'");
            }
        }
    }
}

void SemanticAnalyzer::set_symbol_manager_print(bool print) {
    symbol_manager.set_print_table(print);
}

void SemanticAnalyzer::set_source(char* s []) {
    // Ensure source is not null
    source = s;
    // for (int i = 1; source[i] != nullptr; ++i) {
    //     printf("%s\n", source[i]);
    // }
}

void SemanticAnalyzer::dump_error(int x, int y, std::string message) {
    std::string formatted_error = "<Error> Found in line " + std::to_string(x) +
                                ", column " + std::to_string(y) +
                                ": " + message;
    std::string error_line = source[x];
    error_line = "    " + error_line;
    std::string error_indicator = "    " + std::string(y-1, ' ') + "^";
    error_message.push_back(formatted_error);
    error_message.push_back(error_line);
    error_message.push_back(error_indicator);
}