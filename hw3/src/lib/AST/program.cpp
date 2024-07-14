#include "AST/program.hpp"

// TODO
ProgramNode::ProgramNode(const uint32_t line, const uint32_t col,
                         const char *const p_name, std::vector<DeclNode* > *p_declarations, std::vector<FunctionNode* > *p_functions, CompoundStatementNode *p_compound_statement)
    : AstNode{line, col}, name(p_name), declarations(p_declarations), functions(p_functions), compound_statement(p_compound_statement) {}

// visitor pattern version: const char *ProgramNode::getNameCString() const { return name.c_str(); }

void ProgramNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    if (declarations != NULL)
        for (auto &decl : *declarations)
            decl->accept(p_visitor);
    if (functions != NULL)
        for (auto &func : *functions)
            func->accept(p_visitor);
    compound_statement->accept(p_visitor);
}


// void ProgramNode::visitChildNodes(AstNodeVisitor &p_visitor) { // visitor pattern version
//     /* TODO
//      *
//      * for (auto &decl : var_decls) {
//      *     decl->accept(p_visitor);
//      * }
//      *
//      * // functions
//      *
//      * body->accept(p_visitor);
//      */
// }
