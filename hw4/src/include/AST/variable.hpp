#ifndef AST_VARIABLE_NODE_H
#define AST_VARIABLE_NODE_H

#include "AST/PType.hpp"
#include "AST/ast.hpp"
#include "AST/ConstantValue.hpp"
#include "visitor/AstNodeVisitor.hpp"

#include <memory>
#include <string>

class VariableNode final : public AstNode {
  private:
    std::string m_name;
    PTypeSharedPtr m_type;
    std::shared_ptr<ConstantValueNode> m_constant_value_node_ptr;
    bool is_parameter = false;
    bool is_loop_var = false;

  public:
    ~VariableNode() = default;
    VariableNode(const uint32_t line, const uint32_t col,
                 const std::string &p_name, const PTypeSharedPtr &p_type,
                 const std::shared_ptr<ConstantValueNode> &p_constant_value_node)
        : AstNode{line, col}, m_name(p_name), m_type(p_type),
          m_constant_value_node_ptr(p_constant_value_node) {}

    const char *getNameCString() const { return m_name.c_str(); }
    const char *getTypeCString() const { return m_type->getPTypeCString(); }
    PType* getType() const { return m_type.get(); }
    const char *getKind() const {
        if (m_constant_value_node_ptr) {
            return "constant";
        } else if (is_parameter) {
            return "parameter";
        } else if (is_loop_var) {
            return "loop_var";
        } else {
            return "variable";
        }
    }
    const char *getAttribute() const {
        if (m_constant_value_node_ptr) {
            return m_constant_value_node_ptr->getConstantValueCString();
        } else {
            return "";
        }
    }
    void set_parameter(bool set) { is_parameter = set; }
    void set_loop_var(bool set) { is_loop_var = set; }
    void accept(AstNodeVisitor &p_visitor) override {
        p_visitor.visit(*this);
    }
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

    bool faulty = false;
};

#endif
