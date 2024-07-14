#ifndef __AST_UNARY_OPERATOR_NODE_H
#define __AST_UNARY_OPERATOR_NODE_H

#include "AST/expression.hpp"

class UnaryOperatorNode : public ExpressionNode {
  public:
    UnaryOperatorNode(const uint32_t line, const uint32_t col,
                      std::string p_opertor, ExpressionNode *p_exp
                      /* TODO: operator, expression */);
    ~UnaryOperatorNode() = default;

    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
    void visitChildNodes(AstNodeVisitor &p_visitor) override;
    std::string getOperator() const { return operator_; }
  private:
    // TODO: operator, expression
    std::string operator_;
    ExpressionNode *exp;
};

#endif
