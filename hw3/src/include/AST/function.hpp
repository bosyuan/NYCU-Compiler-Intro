#ifndef __AST_FUNCTION_NODE_H
#define __AST_FUNCTION_NODE_H

#include "AST/ast.hpp"
#include "AST/decl.hpp"
#include "AST/CompoundStatement.hpp"

class FunctionNode : public AstNode {
  public:
    FunctionNode(const uint32_t line, const uint32_t col,
                 const char* name, std::vector<DeclNode*>* parameters,
                 Ptype *p_type, CompoundStatementNode* p_compound_statement                
                 /* TODO: name, declarations, return type,
                  *       compound statement (optional) */);
    ~FunctionNode() = default;

    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
    void visitChildNodes(AstNodeVisitor &p_visitor) override;
    std::string getName() { return name; }
    std::string getPrototype() {
        std::string prototype = return_type + " (";
        if (parameters != NULL) {
            for (auto &parameter : *parameters) {
                prototype += parameter->getPrototype();
                prototype += ", ";
            }
            prototype.pop_back();
            prototype.pop_back();
        }
        prototype += ")";
        return prototype;
    }
  private:
    // TODO: name, declarations, return type, compound statement
    std::string name;
    std::vector<DeclNode*>* parameters;
    std::string return_type;
    CompoundStatementNode* compound_statement;
};

#endif
