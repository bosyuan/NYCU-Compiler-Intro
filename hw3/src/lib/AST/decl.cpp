#include "AST/decl.hpp"

// TODO
DeclNode::DeclNode(const uint32_t line, const uint32_t col, std::vector<VariableNode *> *p_var_name, Ptype *p_type, std::vector<int> *array_size)
    : AstNode{line, col} {
        std::string type;
        if (*p_type == PInt)
            type = "integer";
        else if (*p_type == PReal)
            type = "real";
        else if (*p_type == PString)
            type = "string";
        else if (*p_type == PBoolean)
            type = "boolean";
        if (array_size != nullptr) {
            type += " ";
            for (long unsigned int i = 0; i < array_size->size(); i++) {
                type += "[" + std::to_string(array_size->at(i)) + "]";
            }
        }
        variables = p_var_name;
        for (long unsigned int i = 0; i < variables->size(); i++) {
            variables->at(i)->set_type(type);
            variables->at(i)->set_constant_value(nullptr);
        }
    }

// TODO
DeclNode::DeclNode(const uint32_t line, const uint32_t col, std::vector<VariableNode *> *p_var_name, Ptype *p_type, ConstantValueNode *p_constant_value)
   : AstNode{line, col} {
        std::string type;
        if (*p_type == PInt)
            type = "integer";
        else if (*p_type == PReal)
            type = "real";
        else if (*p_type == PString)
            type = "string";
        else if (*p_type == PBoolean)
            type = "boolean";
        variables = p_var_name;
        for (long unsigned int i = 0; i < variables->size(); i++) {
            variables->at(i)->set_type(type);
            variables->at(i)->set_constant_value(p_constant_value);
        }
   }

DeclNode::DeclNode(const uint32_t line, const uint32_t col, std::vector<VariableNode *> *p_var_name, Ptype *p_type)
   : AstNode{line, col} {
        std::string type;
        if (*p_type == PInt)
            type = "integer";
        else if (*p_type == PReal)
            type = "real";
        else if (*p_type == PString)
            type = "string";
        else if (*p_type == PBoolean)
            type = "boolean";
        variables = p_var_name;
        for (long unsigned int i = 0; i < variables->size(); i++) {
            variables->at(i)->set_type(type);
            variables->at(i)->set_constant_value(nullptr);
        }
   }

void DeclNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    // TODO
    if (variables != NULL)
        for (auto &variable : *variables)
            variable->accept(p_visitor);
}
