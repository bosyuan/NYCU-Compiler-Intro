#include "AST/VariableReference.hpp"

// TODO
VariableReferenceNode::VariableReferenceNode(const uint32_t line,
                                             const uint32_t col,
                                             const char *p_name,
                                             std::vector<ExpressionNode *> *p_exp)

    : ExpressionNode{line, col}, name(p_name), indices(p_exp) {}
VariableReferenceNode::VariableReferenceNode(const uint32_t line,
                                             const uint32_t col,
                                             const char *p_name)
    : ExpressionNode{line, col}, name(p_name), indices(nullptr) {}

// TODO: You may use code snippets in AstDumper.cpp

void VariableReferenceNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    // TODO
    if (indices != nullptr) {
        for (auto &exp : *indices) {
            exp->accept(p_visitor);
        }
    }
}
