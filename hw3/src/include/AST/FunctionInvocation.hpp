#ifndef __AST_FUNCTION_INVOCATION_NODE_H
#define __AST_FUNCTION_INVOCATION_NODE_H

#include "AST/expression.hpp"

class FunctionInvocationNode : public ExpressionNode {
  public:
    FunctionInvocationNode(const uint32_t line, const uint32_t col,
                            std::vector<ExpressionNode* > *p_exp,
                            const char *const p_name
                           /* TODO: function name, expressions */);
    ~FunctionInvocationNode() = default;

    std::string getName() { return name; };
    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

  private:
    // TODO: function name, expressions
    std::vector<ExpressionNode* > *exp;
    std::string name;
};

#endif
