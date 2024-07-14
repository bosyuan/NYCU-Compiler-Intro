#include "AST/UnaryOperator.hpp"

// TODO
UnaryOperatorNode::UnaryOperatorNode(const uint32_t line, const uint32_t col, std::string p_operator, ExpressionNode *p_exp)
    : ExpressionNode{line, col}, operator_(p_operator), exp(p_exp) {}

// TODO: You may use code snippets in AstDumper.cpp

void UnaryOperatorNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    // TODO
    if (exp != NULL)
        exp->accept(p_visitor);
}
