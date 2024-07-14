#ifndef __AST_CONSTANT_VALUE_NODE_H
#define __AST_CONSTANT_VALUE_NODE_H

#include "AST/expression.hpp"
#include "visitor/AstNodeVisitor.hpp"

class ConstantValueNode : public ExpressionNode {
  public:
    ConstantValueNode(const uint32_t line, const uint32_t col,
                      int p_value
                      /* TODO: constant value */);
    ConstantValueNode(const uint32_t line, const uint32_t col,
                      float p_value
                      /* TODO: constant value */);
    ConstantValueNode(const uint32_t line, const uint32_t col,
                      std::string *p_value
                      /* TODO: constant value */);
    ~ConstantValueNode() = default;

    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
    void visitChildNodes(AstNodeVisitor &p_visitor) override {};
    std::string getValue() const { return value; }
  private:
    std::string value;
    // TODO: constant value
};

#endif
