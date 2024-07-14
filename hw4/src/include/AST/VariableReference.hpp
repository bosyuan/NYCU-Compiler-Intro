#ifndef AST_VARIABLE_REFERENCE_NODE_H
#define AST_VARIABLE_REFERENCE_NODE_H

#include "AST/expression.hpp"
#include "visitor/AstNodeVisitor.hpp"
#include "AST/ConstantValue.hpp"

#include <memory>
#include <string>
#include <vector>

class VariableReferenceNode final : public ExpressionNode {
  public:
    using ExprNodes = std::vector<std::unique_ptr<ExpressionNode>>;

  private:
    std::string m_name;
    ExprNodes m_indices;
    std::vector<const char*> m_dimensions;

  public:
    ~VariableReferenceNode() = default;

    // normal reference
    VariableReferenceNode(const uint32_t line, const uint32_t col,
                          const char *const p_name)
        : ExpressionNode{line, col}, m_name(p_name){}

    // array reference
    VariableReferenceNode(const uint32_t line, const uint32_t col,
                          const char *const p_name, ExprNodes &p_indices)
        : ExpressionNode{line, col}, m_name(p_name),
          m_indices(std::move(p_indices)){}

    const char *getNameCString() const { return m_name.c_str(); }
    ExprNodes* get_nodes() { return &m_indices; }
    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
    void visitChildNodes(AstNodeVisitor &p_visitor) override;
    std::vector<const char*> getDimensions() const { return m_dimensions; }
    Location get_child_location(int index);
    bool checkIndiceValid() {
        for (auto &index : m_indices) {
            if (index->get_type() == nullptr) return false;
        }
        for (auto &index : m_indices) {
            m_dimensions.push_back(static_cast<ConstantValueNode*>(index.get())->getConstantValueCString());
        }
        return true;
    }
};

#endif
