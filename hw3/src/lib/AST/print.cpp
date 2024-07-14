#include "AST/print.hpp"

// TODO
PrintNode::PrintNode(const uint32_t line, const uint32_t col, ExpressionNode *p_exp)
    : AstNode{line, col}, target(p_exp) {}

// TODO: You may use code snippets in AstDumper.cpp

void PrintNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    // TODO
    if (target != NULL){
        target->accept(p_visitor);
    }
}
