#ifndef __AST_VARIABLE_NODE_H
#define __AST_VARIABLE_NODE_H

#include "AST/ast.hpp"
#include "AST/ConstantValue.hpp"

class VariableNode : public AstNode {
  public:
    VariableNode(const uint32_t line, const uint32_t col,
                  std::string p_name, std::string p_type,
                  ConstantValueNode *p_constant_value
                 /* TODO: variable name, type, constant value */);
    VariableNode(const uint32_t line, const uint32_t col,
                  std::string p_name);
    ~VariableNode() = default;
    
    void set_type(std::string p_type) { type = p_type; }
    void set_constant_value(ConstantValueNode *p_constant_value) { constant_value = p_constant_value; }
    std::string getName() const { return name; }
    std::string getType() const { return type; }
    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

  private:
    // TODO: variable name, type, constant value
    std::string name;
    std::string type;
    ConstantValueNode *constant_value;
};

#endif
