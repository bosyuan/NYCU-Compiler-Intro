#include "AST/function.hpp"

// TODO
FunctionNode::FunctionNode(const uint32_t line, const uint32_t col, const char* name, std::vector<DeclNode*>* parameters, Ptype *p_return_type, CompoundStatementNode* p_compound_statement)
    : AstNode{line, col} {
        this->name = name;
        this->parameters = parameters;
        this->compound_statement = p_compound_statement;

        if (*p_return_type == PInt)
            this->return_type = "integer";
        else if (*p_return_type == PReal)
            this->return_type = "real";
        else if (*p_return_type == PString)
            this->return_type = "string";
        else if (*p_return_type == PBoolean)
            this->return_type = "boolean";
        else if (*p_return_type == PVoid)
            this->return_type = "void";
    }

// TODO: You may use code snippets in AstDumper.cpp
void FunctionNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    // TODO
    if (parameters != NULL) {
        for (auto &parameter : *parameters) {
            parameter->accept(p_visitor);
        }
    }
    if (compound_statement != NULL)
        compound_statement->accept(p_visitor);
}
