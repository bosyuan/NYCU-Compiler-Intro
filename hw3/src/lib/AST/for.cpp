#include "AST/for.hpp"

// TODO
ForNode::ForNode(const uint32_t line, const uint32_t col, DeclNode *p_decl, AssignmentNode *p_assign, ConstantValueNode *p_exp, CompoundStatementNode *p_compound_statement)
    : AstNode{line, col}, loop_varialbe_declaration(p_decl), initial_statement(p_assign), condition(p_exp), body(p_compound_statement) {}

// TODO: You may use code snippets in AstDumper.cpp

void ForNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    // TODO
    if (loop_varialbe_declaration != nullptr) {
        loop_varialbe_declaration->accept(p_visitor);
    }
    if (initial_statement != nullptr) {
        initial_statement->accept(p_visitor);
    }
    if (condition != nullptr) {
        condition->accept(p_visitor);
    }
    if (body != nullptr) {
        body->accept(p_visitor);
    }
}
