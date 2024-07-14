#include "AST/if.hpp"

// TODO
IfNode::IfNode(const uint32_t line, const uint32_t col, ExpressionNode *p_exp, CompoundStatementNode *p_compound_statement, CompoundStatementNode *p_compound_statement_else)
    : AstNode{line, col}, exp(p_exp), body(p_compound_statement), body_else(p_compound_statement_else) {}

// TODO: You may use code snippets in AstDumper.cpp

void IfNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    // TODO
    if (exp != nullptr) {
        exp->accept(p_visitor);
    }
    if (body != nullptr) {
        body->accept(p_visitor);
    }
    if (body_else != nullptr) {
        body_else->accept(p_visitor);
    }
}
