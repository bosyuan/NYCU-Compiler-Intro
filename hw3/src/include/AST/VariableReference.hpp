#ifndef __AST_VARIABLE_REFERENCE_NODE_H
#define __AST_VARIABLE_REFERENCE_NODE_H

#include "AST/expression.hpp"

class VariableReferenceNode : public ExpressionNode {
  public:
    // array reference
    VariableReferenceNode(const uint32_t line, const uint32_t col,
                          const char *const p_name, std::vector<ExpressionNode* > *p_exp
                          /* TODO: name */);
    // normal reference
    VariableReferenceNode(const uint32_t line, const uint32_t col, const char *const p_name
                          /* TODO: name, expressions */);
    ~VariableReferenceNode() = default;
    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
    void visitChildNodes(AstNodeVisitor &p_visitor) override;
    std::string getName() {
        return name;
    }


  private:
    // TODO: variable name, expressions
    std::string name;
    std::vector<ExpressionNode* > *indices;
};

#endif
