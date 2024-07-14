#include "AST/assignment.hpp"

// TODO
AssignmentNode::AssignmentNode(const uint32_t line, const uint32_t col, VariableReferenceNode *p_var_ref, ExpressionNode *p_exp)
    : AstNode{line, col}, Lvalue(p_var_ref), expression(p_exp) {}

// TODO: You may use code snippets in AstDumper.cpp

void AssignmentNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    // TODO
    if (Lvalue != nullptr) {
        Lvalue->accept(p_visitor);
    }
    if (expression != nullptr) {
        expression->accept(p_visitor);
    }
}
