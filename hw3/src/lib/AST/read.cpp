#include "AST/read.hpp"

// TODO
ReadNode::ReadNode(const uint32_t line, const uint32_t col, VariableReferenceNode *p_var_ref)
    : AstNode{line, col}, target(p_var_ref) {}

// TODO: You may use code snippets in AstDumper.cpp

void ReadNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    // TODO
    if (target != nullptr) {
        target->accept(p_visitor);
    }
}
