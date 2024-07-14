#ifndef __AST_BINARY_OPERATOR_NODE_H
#define __AST_BINARY_OPERATOR_NODE_H

#include "AST/expression.hpp"

#include <memory>

class BinaryOperatorNode : public ExpressionNode {
  public:
    BinaryOperatorNode(const uint32_t line, const uint32_t col,
                       std::string p_operator, ExpressionNode *p_lhs, ExpressionNode *p_rhs
                       /* TODO: operator, expressions */);
    ~BinaryOperatorNode() = default;
    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
    void visitChildNodes(AstNodeVisitor &p_visitor) override;
    std::string getOperator() const { return operator_; }
  private:
    // TODO: operator, expressions
    std::string operator_;
    ExpressionNode *lhs, *rhs;
};

#endif
