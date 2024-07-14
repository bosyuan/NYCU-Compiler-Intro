#ifndef SEMA_SEMANTIC_ANALYZER_H
#define SEMA_SEMANTIC_ANALYZER_H

#include "visitor/AstNodeVisitor.hpp"
#include "AST/PType.hpp"
// #include "sema/SymbolTable.hpp"

#include <vector>
#include <stack>
#include <string>
#include <string.h>

class SymbolEntry {
public:
    const char* name;
    const char* kind;
    int level;
    PType* type;
    const char* attribute;
};

class SymbolTable {
public:
    SymbolTable() = default;
    SymbolTable(PType* return_type) : return_type(return_type) {};
    void addSymbol(const char* name, const char* kind, int level, PType* type, const char* attribute);
    bool lookup(const char* name);
    SymbolEntry* lookup_local(const char* name);
    // other methods
    std::vector<SymbolEntry> entries;
    PType* return_type = nullptr;
};

class SymbolManager {
public:
    SymbolManager() = default;
    void pushScope();
    void pushScope(SymbolTable *new_scope);
    void popScope();
    void printCurrentScope();
    void set_print_table(bool print);
    void pushSymbol(const char* name, const char* kind, int level, PType* type, const char* attribute);
    bool lookup(const char* name) {return tables.top()->lookup(name);};
    SymbolEntry* lookup_global(const char* name);
    PType* get_top_return_type() {return tables.top()->return_type;};
    
    int get_level(){return tables.size() -1;};
    // other methods
private:
    std::stack <SymbolTable *> tables;
    bool print_table = true;
};

class SemanticAnalyzer final : public AstNodeVisitor {
  private:
    // TODO: something like symbol manager (manage symbol tables)
    //       context manager, return type manager
    SymbolManager symbol_manager;
    char** source;
    std::vector<std::string> error_message;
    std::vector<std::string> loop_variable;
    int loop_variable_level = 0;

  public:
    ~SemanticAnalyzer() = default;
    SemanticAnalyzer() = default;

    void visit(ProgramNode &p_program) override;
    void visit(DeclNode &p_decl) override;
    void visit(VariableNode &p_variable) override;
    void visit(ConstantValueNode &p_constant_value) override;
    void visit(FunctionNode &p_function) override;
    void visit(CompoundStatementNode &p_compound_statement) override;
    void visit(PrintNode &p_print) override;
    void visit(BinaryOperatorNode &p_bin_op) override;
    void visit(UnaryOperatorNode &p_un_op) override;
    void visit(FunctionInvocationNode &p_func_invocation) override;
    void visit(VariableReferenceNode &p_variable_ref) override;
    void visit(AssignmentNode &p_assignment) override;
    void visit(ReadNode &p_read) override;
    void visit(IfNode &p_if) override;
    void visit(WhileNode &p_while) override;
    void visit(ForNode &p_for) override;
    void visit(ReturnNode &p_return) override;

    void set_symbol_manager_print(bool print);
    void set_source(char* s[]);
    void dump_error(int x, int y, std::string message);
};

#endif
