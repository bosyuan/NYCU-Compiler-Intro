#include "AST/return.hpp"

// TODO
ReturnNode::ReturnNode(const uint32_t line, const uint32_t col, ExpressionNode *p_exp)
    : AstNode{line, col}, return_value(p_exp) {}

// TODO: You may use code snippets in AstDumper.cpp

void ReturnNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    // TODO
    if (return_value != nullptr) {
        return_value->accept(p_visitor);
    }
}
