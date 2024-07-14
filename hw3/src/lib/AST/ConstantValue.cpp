#include "AST/ConstantValue.hpp"

// TODO
ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col, int p_value)
    : ExpressionNode{line, col}, value(std::to_string(p_value)) {}
ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col, float p_value)
    : ExpressionNode{line, col}, value(std::to_string(p_value)) {}
ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col, std::string *p_value)
    : ExpressionNode{line, col}, value(*p_value) {}
// TODO: You may use code snippets in AstDumper.cpp
