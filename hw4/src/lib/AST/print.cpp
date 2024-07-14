#include "AST/print.hpp"

void PrintNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    m_target->accept(p_visitor);
}

bool PrintNode::checkScalarType() {
    if (m_target->get_type()->isScalarType()) {
        return true;
    }
    return false;
}