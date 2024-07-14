#ifndef __AST_FOR_NODE_H
#define __AST_FOR_NODE_H

#include "AST/ast.hpp"
#include "AST/decl.hpp"
#include "AST/assignment.hpp"
#include "AST/ConstantValue.hpp"
#include "AST/CompoundStatement.hpp"

class ForNode : public AstNode {
  public:
    ForNode(const uint32_t line, const uint32_t col, DeclNode *p_decl, AssignmentNode *p_assign, ConstantValueNode *p_exp, CompoundStatementNode *p_compound_statement
            /* TODO: declaration, assignment, expression,
             *       compound statement */);
    ~ForNode() = default;
    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

  private:
    // TODO: declaration, assignment, expression, compound statement
    DeclNode *loop_varialbe_declaration;
    AssignmentNode *initial_statement;
    ConstantValueNode *condition;
    CompoundStatementNode *body;
};

#endif
