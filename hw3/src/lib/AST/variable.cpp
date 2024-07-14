#include "AST/variable.hpp"

// TODO
VariableNode::VariableNode(const uint32_t line, const uint32_t col, std::string p_name, std::string p_type, ConstantValueNode *p_constant_value)
    : AstNode{line, col}, name(p_name), type(p_type), constant_value(p_constant_value) {}

VariableNode::VariableNode(const uint32_t line, const uint32_t col, std::string p_name)
    : AstNode{line, col}, name(p_name) {}


void VariableNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    // TODO
    if (constant_value != nullptr)
        constant_value->accept(p_visitor);
}
