#ifndef __AST_IF_NODE_H
#define __AST_IF_NODE_H

#include "AST/ast.hpp"
#include "AST/expression.hpp"
#include "AST/CompoundStatement.hpp"

class IfNode : public AstNode {
  public:
    IfNode(const uint32_t line, const uint32_t col, ExpressionNode *p_exp, CompoundStatementNode *p_compound_statement, CompoundStatementNode *p_compound_statement_else
           /* TODO: expression, compound statement, compound statement */);
    ~IfNode() = default;
    
    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

  private:
    // TODO: expression, compound statement, compound statement
    ExpressionNode *exp;
    CompoundStatementNode *body;
    CompoundStatementNode *body_else;
};

#endif
