#include "AST/UnaryOperator.hpp"

void UnaryOperatorNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    auto visit_ast_node = [&](auto &ast_node) { ast_node->accept(p_visitor); };

    visit_ast_node(m_operand);
}

bool UnaryOperatorNode::validateType() {
    switch(m_op) {
        case Operator::kNotOp:
            // check if operand is boolean
            if (m_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kBoolType) {
                return false;
            }
            else if (m_operand->get_type()->getDimensions().size() != 0) {
                return false;
            }
            // set type to real if either operand is real
            this->set_type(std::make_shared<PType>(PType::PrimitiveTypeEnum::kBoolType));
        break;
        case Operator::kNegOp:
            // check if operand is boolean
            if (m_operand->get_type()->getPrimitiveType() == PType::PrimitiveTypeEnum::kRealType && m_operand->get_type()->getDimensions().size() == 0) {
                this->set_type(std::make_shared<PType>(PType::PrimitiveTypeEnum::kRealType));
                return true;
            }
            else if (m_operand->get_type()->getPrimitiveType() == PType::PrimitiveTypeEnum::kIntegerType && m_operand->get_type()->getDimensions().size() == 0) {
                this->set_type(std::make_shared<PType>(PType::PrimitiveTypeEnum::kIntegerType));
                return true;
            }
            else {
                return false;
            }
        break;
        default:
            return false;
    }
    return true;
}