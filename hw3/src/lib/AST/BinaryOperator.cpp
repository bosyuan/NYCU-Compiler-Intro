#include "AST/BinaryOperator.hpp"

// TODO
BinaryOperatorNode::BinaryOperatorNode(const uint32_t line, const uint32_t col, std::string p_operator, ExpressionNode *p_lhs, ExpressionNode *p_rhs)
    : ExpressionNode{line, col}, operator_(p_operator), lhs(p_lhs), rhs(p_rhs) {}

// TODO: You may use code snippets in AstDumper.cpp

void BinaryOperatorNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    // TODO
    if (lhs != NULL)
        lhs->accept(p_visitor);
    if (rhs != NULL)
        rhs->accept(p_visitor);
}
