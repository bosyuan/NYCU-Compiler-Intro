#include "AST/BinaryOperator.hpp"

void BinaryOperatorNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    auto visit_ast_node = [&](auto &ast_node) { ast_node->accept(p_visitor); };

    visit_ast_node(m_left_operand);
    visit_ast_node(m_right_operand);
}

bool BinaryOperatorNode::validateType() {
    switch(m_op) {
        case Operator::kPlusOp:
            if (m_left_operand->get_type()->getPrimitiveType() == PType::PrimitiveTypeEnum::kStringType && m_right_operand->get_type()->getPrimitiveType() == PType::PrimitiveTypeEnum::kStringType) {
                this->set_type(std::make_shared<PType>(PType::PrimitiveTypeEnum::kStringType));
                return true;
            }
            // check if both operands are integer or real
            if (m_left_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kRealType && m_left_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kIntegerType) {
                return false;
            }
            else if (m_right_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kRealType && m_right_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kIntegerType) {
                return false;
            }
            else if (m_left_operand->get_type()->getDimensions().size() != 0 || m_right_operand->get_type()->getDimensions().size() != 0) {
                return false;
            }
            // set type to real if either operand is real
            if (m_left_operand->get_type()->getPrimitiveType() == PType::PrimitiveTypeEnum::kRealType || m_right_operand->get_type()->getPrimitiveType() == PType::PrimitiveTypeEnum::kRealType) {
                this->set_type(std::make_shared<PType>(PType::PrimitiveTypeEnum::kRealType));
            }
            else {
                this->set_type(std::make_shared<PType>(PType::PrimitiveTypeEnum::kIntegerType));
            }
        break;
        case Operator::kMinusOp:
            // check if both operands are integer or real
            if (m_left_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kRealType && m_left_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kIntegerType) {
                return false;
            }
            else if (m_right_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kRealType && m_right_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kIntegerType) {
                return false;
            }
            else if (m_left_operand->get_type()->getDimensions().size() != 0 || m_right_operand->get_type()->getDimensions().size() != 0) {
                return false;
            }
            // set type to real if either operand is real
            if (m_left_operand->get_type()->getPrimitiveType() == PType::PrimitiveTypeEnum::kRealType || m_right_operand->get_type()->getPrimitiveType() == PType::PrimitiveTypeEnum::kRealType) {
                this->set_type(std::make_shared<PType>(PType::PrimitiveTypeEnum::kRealType));
            }
            else {
                this->set_type(std::make_shared<PType>(PType::PrimitiveTypeEnum::kIntegerType));
            }
        break;
        case Operator::kMultiplyOp:
            // check if both operands are integer or real
            if (m_left_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kRealType && m_left_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kIntegerType) {
                return false;
            }
            else if (m_right_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kRealType && m_right_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kIntegerType) {
                return false;
            }
            else if (m_left_operand->get_type()->getDimensions().size() != 0 || m_right_operand->get_type()->getDimensions().size() != 0) {
                return false;
            }
            // set type to real if either operand is real
            if (m_left_operand->get_type()->getPrimitiveType() == PType::PrimitiveTypeEnum::kRealType || m_right_operand->get_type()->getPrimitiveType() == PType::PrimitiveTypeEnum::kRealType) {
                this->set_type(std::make_shared<PType>(PType::PrimitiveTypeEnum::kRealType));
            }
            else {
                this->set_type(std::make_shared<PType>(PType::PrimitiveTypeEnum::kIntegerType));
            }
        break;
        case Operator::kDivideOp:
            // check if both operands are integer or real
            if (m_left_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kRealType && m_left_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kIntegerType) {
                return false;
            }
            else if (m_right_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kRealType && m_right_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kIntegerType) {
                return false;
            }
            else if (m_left_operand->get_type()->getDimensions().size() != 0 || m_right_operand->get_type()->getDimensions().size() != 0) {
                return false;
            }
            // set type to real after division
            this->set_type(std::make_shared<PType>(PType::PrimitiveTypeEnum::kIntegerType));
        break;
        case Operator::kModOp:
            // check if both operands are integer
            if (m_left_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kIntegerType) {
                return false;
            }
            else if (m_right_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kIntegerType) {
                return false;
            }
            else if (m_left_operand->get_type()->getDimensions().size() != 0 || m_right_operand->get_type()->getDimensions().size() != 0) {
                return false;
            }
            this->set_type(std::make_shared<PType>(PType::PrimitiveTypeEnum::kIntegerType));
        break;
        case Operator::kAndOp:
            // check if both operands are bool
            if (m_left_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kBoolType) {
                return false;
            }
            else if (m_right_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kBoolType) {
                return false;
            }
            else if (m_left_operand->get_type()->getDimensions().size() != 0 || m_right_operand->get_type()->getDimensions().size() != 0) {
                return false;
            }
            this->set_type(std::make_shared<PType>(PType::PrimitiveTypeEnum::kBoolType));
        break;
        case Operator::kOrOp:
            // check if both operands are bool
            if (m_left_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kBoolType) {
                return false;
            }
            else if (m_right_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kBoolType) {
                return false;
            }
            else if (m_left_operand->get_type()->getDimensions().size() != 0 || m_right_operand->get_type()->getDimensions().size() != 0) {
                return false;
            }
            this->set_type(std::make_shared<PType>(PType::PrimitiveTypeEnum::kBoolType));
        break;
        case Operator::kEqualOp:
            // check if both operands are integer or real
            if (m_left_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kRealType && m_left_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kIntegerType) {
                return false;
            }
            else if (m_right_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kRealType && m_right_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kIntegerType) {
                return false;
            }
            else if (m_left_operand->get_type()->getDimensions().size() != 0 || m_right_operand->get_type()->getDimensions().size() != 0) {
                return false;
            }
            this->set_type(std::make_shared<PType>(PType::PrimitiveTypeEnum::kBoolType));
        break;
        case Operator::kGreaterOp:
            // check if both operands are integer or real
            if (m_left_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kRealType && m_left_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kIntegerType) {
                return false;
            }
            else if (m_right_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kRealType && m_right_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kIntegerType) {
                return false;
            }
            else if (m_left_operand->get_type()->getDimensions().size() != 0 || m_right_operand->get_type()->getDimensions().size() != 0) {
                return false;
            }
            this->set_type(std::make_shared<PType>(PType::PrimitiveTypeEnum::kBoolType));
        break;
        case Operator::kGreaterOrEqualOp:
            // check if both operands are integer or real
            if (m_left_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kRealType && m_left_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kIntegerType) {
                return false;
            }
            else if (m_right_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kRealType && m_right_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kIntegerType) {
                return false;
            }
            else if (m_left_operand->get_type()->getDimensions().size() != 0 || m_right_operand->get_type()->getDimensions().size() != 0) {
                return false;
            }
            this->set_type(std::make_shared<PType>(PType::PrimitiveTypeEnum::kBoolType));
        break;
        case Operator::kLessOp:
            // check if both operands are integer or real
            if (m_left_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kRealType && m_left_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kIntegerType) {
                return false;
            }
            else if (m_right_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kRealType && m_right_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kIntegerType) {
                return false;
            }
            else if (m_left_operand->get_type()->getDimensions().size() != 0 || m_right_operand->get_type()->getDimensions().size() != 0) {
                return false;
            }
            this->set_type(std::make_shared<PType>(PType::PrimitiveTypeEnum::kBoolType));
        break;
        case Operator::kLessOrEqualOp:
            // check if both operands are integer or real
            if (m_left_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kRealType && m_left_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kIntegerType) {
                return false;
            }
            else if (m_right_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kRealType && m_right_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kIntegerType) {
                return false;
            }
            else if (m_left_operand->get_type()->getDimensions().size() != 0 || m_right_operand->get_type()->getDimensions().size() != 0) {
                return false;
            }
            this->set_type(std::make_shared<PType>(PType::PrimitiveTypeEnum::kBoolType));
        break;
        case Operator::kNotEqualOp:
            // check if both operands are integer or real
            if (m_left_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kRealType && m_left_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kIntegerType) {
                return false;
            } 
            else if (m_right_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kRealType && m_right_operand->get_type()->getPrimitiveType() != PType::PrimitiveTypeEnum::kIntegerType) {
                return false;
            } 
            else if (m_left_operand->get_type()->getDimensions().size() != 0 || m_right_operand->get_type()->getDimensions().size() != 0){
                return false;
            }
            this->set_type(std::make_shared<PType>(PType::PrimitiveTypeEnum::kBoolType));
        break;
        default:
            return false;
    }
    return true;
}