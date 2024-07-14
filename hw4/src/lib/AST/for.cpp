#include "AST/for.hpp"

void ForNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    m_loop_var_decl->set_loop_var(true);
    m_loop_var_decl->accept(p_visitor);
    if (!m_loop_var_decl->faulty) {
        m_init_stmt->accept(p_visitor);
        m_end_condition->accept(p_visitor);
    }
    m_body->accept(p_visitor);
}

bool ForNode::variableBoundCheck() {
    ConstantValueNode* end = dynamic_cast<ConstantValueNode*>(m_end_condition.get());
    ConstantValueNode* init = dynamic_cast<ConstantValueNode*>(m_init_stmt->getExpr());
    int e = atoi(end->getConstantValueCString());
    int i = atoi(init->getConstantValueCString());
    if (e < i) {
        return false;
    }
    return true;
}