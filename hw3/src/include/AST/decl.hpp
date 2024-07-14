#ifndef __AST_DECL_NODE_H
#define __AST_DECL_NODE_H

#include "AST/ast.hpp"
#include "AST/variable.hpp"
#include "visitor/AstNodeVisitor.hpp"

class DeclNode : public AstNode {
  public:
    // variable declaration
    DeclNode(const uint32_t line, const uint32_t col,
            std::vector<VariableNode *> *p_var_name, Ptype *p_type, std::vector<int> *array_size
             /* TODO: identifiers, type */);

    // constant variable declaration
    DeclNode(const uint32_t, const uint32_t col,
            std::vector<VariableNode *> *p_var_name, Ptype *p_type, ConstantValueNode *p_constant_value
            /* TODO: identifiers, constant */);
    DeclNode(const uint32_t, const uint32_t col,
            std::vector<VariableNode *> *p_var_name, Ptype *p_type
            /* TODO: identifiers, constant */);

    ~DeclNode() = default;

    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
    void visitChildNodes(AstNodeVisitor &p_visitor) override;
    std::string getPrototype() {
      std::string prototype = "";
      if (variables != nullptr) {
        for (auto &variable : *variables) {
          prototype += variable->getType();
          prototype += ", ";
        }
        prototype.pop_back();
        prototype.pop_back();
      }
      return prototype;
    }
  private:
    // TODO: variables
    std::vector<VariableNode* > *variables;
};

#endif
