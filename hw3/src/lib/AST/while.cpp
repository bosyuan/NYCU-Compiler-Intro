#include "AST/while.hpp"

// TODO
WhileNode::WhileNode(const uint32_t line, const uint32_t col, ExpressionNode *p_expression, CompoundStatementNode *p_compound_statement)
    : AstNode{line, col}, condition(p_expression), body(p_compound_statement) {}

// TODO: You may use code snippets in AstDumper.cpp

void WhileNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    // TODO
    condition->accept(p_visitor);
    if (body != NULL)
        body->accept(p_visitor);
}
