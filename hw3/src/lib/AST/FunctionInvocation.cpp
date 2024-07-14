#include "AST/FunctionInvocation.hpp"

// TODO
FunctionInvocationNode::FunctionInvocationNode(const uint32_t line,
                                               const uint32_t col,
                                               std::vector<ExpressionNode *> *p_exp,
                                               const char *const p_name)
    : ExpressionNode{line, col}, exp(p_exp), name(p_name) {}

// TODO: You may use code snippets in AstDumper.cpp

void FunctionInvocationNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    // TODO
    if (exp != NULL){
        for (auto &it : *exp) {
            it->accept(p_visitor);
        }
    }
}
