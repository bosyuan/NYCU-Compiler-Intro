#include "AST/VariableReference.hpp"

#include <algorithm>

void VariableReferenceNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    auto visit_ast_node = [&](auto &ast_node) { ast_node->accept(p_visitor); };

    for_each(m_indices.begin(), m_indices.end(), visit_ast_node);
}

Location VariableReferenceNode::get_child_location(int index) {
    return m_indices[index]->getLocation();
}