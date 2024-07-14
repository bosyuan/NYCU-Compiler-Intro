#include "codegen/CodeGenerator.hpp"
#include "visitor/AstNodeInclude.hpp"
#include "AST/operator.hpp"

#include <algorithm>
#include <cassert>
#include <cstdarg>
#include <cstdio>

CodeGenerator::CodeGenerator(const std::string &source_file_name,
                             const std::string &save_path,
                             const SymbolManager *const p_symbol_manager)
    : m_symbol_manager_ptr(p_symbol_manager),
      m_source_file_path(source_file_name) {
    // FIXME: assume that the source file is always xxxx.p
    const auto &real_path =
        save_path.empty() ? std::string{"."} : save_path;
    auto slash_pos = source_file_name.rfind("/");
    auto dot_pos = source_file_name.rfind(".");

    if (slash_pos != std::string::npos) {
        ++slash_pos;
    } else {
        slash_pos = 0;
    }
    auto output_file_path{
        real_path + "/" +
        source_file_name.substr(slash_pos, dot_pos - slash_pos) + ".S"};
    m_output_file.reset(fopen(output_file_path.c_str(), "w"));
    assert(m_output_file.get() && "Failed to open output file");
    is_global_constant = false;
    is_main = false;
    is_lvalue = false;
    is_if = false;
    is_else = false;
    is_branch = false;
    is_while = false;
    is_for = false;
    is_for_head = false;
    label_num = 0;
    current_label = 0;
    local_addr = 8;
    parameter_num = 0;
}

static void dumpInstructions(FILE *p_out_file, const char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(p_out_file, format, args);
    va_end(args);
}

void CodeGenerator::visit(ProgramNode &p_program) {
    // Generate RISC-V instructions for program header
    // clang-format off
    constexpr const char *const riscv_assembly_file_prologue =
        "    .file \"%s\"\n"
        "    .option nopic\n";

    // clang-format on
    dumpInstructions(m_output_file.get(), riscv_assembly_file_prologue,
                     m_source_file_path.c_str());

    // Reconstruct the hash table for looking up the symbol entry
    // Hint: Use symbol_manager->lookup(symbol_name) to get the symbol entry.
    m_symbol_manager_ptr->reconstructHashTableFromSymbolTable(
        p_program.getSymbolTable());

    auto visit_ast_node = [&](auto &ast_node) { ast_node->accept(*this); };
    for_each(p_program.getDeclNodes().begin(), p_program.getDeclNodes().end(),
             visit_ast_node);
    for_each(p_program.getFuncNodes().begin(), p_program.getFuncNodes().end(),
             visit_ast_node);

    is_main = true;
    const_cast<CompoundStatementNode &>(p_program.getBody()).accept(*this);
    constexpr const char*const riscv_assembly_function_epilogue =
        "   # in the function epilogue\n"
        "   lw ra, 124(sp)      # load return address saved in the current stack\n"
        "   lw s0, 120(sp)      # move frame pointer back to the bottom of the last stack\n"
        "   addi sp, sp, 128    # move stack pointer back to the top of the last stack\n"
        "   jr ra               # jump back to the caller function\n"
        "   .size main, .-main\n\n";

    dumpInstructions(m_output_file.get(), riscv_assembly_function_epilogue);

    // Remove the entries in the hash table
    m_symbol_manager_ptr->removeSymbolsFromHashTable(p_program.getSymbolTable());
    fclose(m_output_file.get());
}

void CodeGenerator::visit(DeclNode &p_decl) {
    p_decl.visitChildNodes(*this);
}

void CodeGenerator::visit(VariableNode &p_variable) {
    const SymbolEntry *symbol_entry = m_symbol_manager_ptr->lookup(p_variable.getName());
    if (symbol_entry == nullptr) return;

    if (symbol_entry->getLevel() == 0) {
        if (symbol_entry->getKind() == SymbolEntry::KindEnum::kVariableKind) {
            constexpr const char *const riscv_assembly_file_global_variable =
                "#declare global variable\n"
                ".comm %s, 4, 4    # emit object '%s' to .bss section with size = 4, align = 4\n";
            dumpInstructions(m_output_file.get(), riscv_assembly_file_global_variable, p_variable.getNameCString(), p_variable.getNameCString());
        }
        else if (symbol_entry->getKind() == SymbolEntry::KindEnum::kConstantKind) {
            constexpr const char*const riscv_assembly_global_const_expr =
            "#declare global constant\n"
            ".section    .rodata\n"
            "   .align 2\n"
            "   .globl %s\n"
            "   .type %s, @object\n"
            "%s:\n"
            "    .word ";
            dumpInstructions(m_output_file.get(), riscv_assembly_global_const_expr, p_variable.getNameCString(), p_variable.getNameCString(), p_variable.getNameCString(), p_variable.getNameCString());
            is_global_constant = true;
            p_variable.visitChildNodes(*this);
        }
    }

    else if (symbol_entry->getKind() == SymbolEntry::KindEnum::kVariableKind || symbol_entry->getKind() == SymbolEntry::KindEnum::kLoopVarKind) {
        addrStackPush(p_variable.getName());
        local_addr += 4;
    }
    else if (symbol_entry->getKind() == SymbolEntry::KindEnum::kConstantKind) {
        addrStackPush(symbol_entry->getName());
        // printf("save %s to stack %d\n", symbol_entry->getNameCString(), local_addr);
        local_addr += 4;
        constexpr const char*const riscv_assembly_local_const_expr=
            "#declare local constant\n"
            "   addi t0, s0, -%d\n"
            "   addi sp, sp, -4\n"
            "   sw t0, 0(sp)        # push the address to the stack\n";
        dumpInstructions(m_output_file.get(), riscv_assembly_local_const_expr, local_addr);
        p_variable.visitChildNodes(*this);
        constexpr const char*const riscv_assembly_local_const_expr2=
            "   lw t0, 0(sp)        # pop the value from the stack\n"
            "   addi sp, sp, 4\n"
            "   lw t1, 0(sp)        # pop the address from the stack\n"
            "   addi sp, sp, 4\n"
            "   sw t0, 0(t1)\n";
        dumpInstructions(m_output_file.get(), riscv_assembly_local_const_expr2);
    }
    else if (symbol_entry->getKind() == SymbolEntry::KindEnum::kParameterKind) {
        addrStackPush(p_variable.getName());
        constexpr const char*const riscv_assembly_func_paras_expr =
        "   sw a%d, -%d(s0)      # save parameter %s in the local stack\n";
        dumpInstructions(m_output_file.get(), riscv_assembly_func_paras_expr, parameter_num, local_addr+4, p_variable.getNameCString());
        local_addr += 4;
        parameter_num++;
    }
}

void CodeGenerator::visit(ConstantValueNode &p_constant_value) {
    if (is_global_constant) {
        constexpr const char*const riscv_assembly_global_const_expr =
        "%s\n";
        dumpInstructions(m_output_file.get(), riscv_assembly_global_const_expr, p_constant_value.getConstantValueCString());
        is_global_constant = false;
    }
    else {
        constexpr const char*const riscv_assembly_const_expr = 
        "   li t0, %s            # load value to register 't0'\n"
        "   addi sp, sp, -4\n"
        "   sw t0, 0(sp)        # push the value to the stack\n";
        dumpInstructions(m_output_file.get(), riscv_assembly_const_expr, p_constant_value.getConstantValueCString());
    }
}

void CodeGenerator::visit(FunctionNode &p_function) {
    // Reconstruct the hash table for looking up the symbol entry
    m_symbol_manager_ptr->reconstructHashTableFromSymbolTable(
        p_function.getSymbolTable());
    
    local_addr = 8;
    is_main = false;
    constexpr const char*const riscv_assembly_function_prologue =
        ".section    .text\n"
        "    .align 2\n"
        "   .globl %s         # emit symbol '%s' to the global symbol table\n"
        "   .type %s, @function\n\n"
        "%s:\n"
        "   # in the function prologue\n"
        "   addi sp, sp, -128   # move stack pointer to lower address to allocate a new stack\n"
        "   sw ra, 124(sp)      # save return address of the caller function in the current stack\n"
        "   sw s0, 120(sp)      # save frame pointer of the last stack in the current stack\n"
        "   addi s0, sp, 128    # move frame pointer to the bottom of the current stack\n\n";
    dumpInstructions(m_output_file.get(), riscv_assembly_function_prologue, p_function.getNameCString(), p_function.getNameCString(), p_function.getNameCString(), p_function.getNameCString());
    p_function.visitChildNodes(*this);
    is_main = true;
    local_addr = 8;
    parameter_num = 0;

    constexpr const char*const riscv_assembly_function_epilogue =
        "   # in the function epilogue\n"
        "   lw ra, 124(sp)      # load return address saved in the current stack\n"
        "   lw s0, 120(sp)      # move frame pointer back to the bottom of the last stack\n"
        "   addi sp, sp, 128    # move stack pointer back to the top of the last stack\n"
        "   jr ra               # jump back to the caller function\n"
        "   .size %s, .-%s\n\n";
    dumpInstructions(m_output_file.get(), riscv_assembly_function_epilogue, p_function.getNameCString(), p_function.getNameCString());

    //restore main symbol table
    if (p_function.getSymbolTable() != nullptr) {
        const auto &entries = p_function.getSymbolTable()->getEntries();
        for (auto &entry : entries) {
            addrStackPop(entry->getName());
        }
    }
    // Remove the entries in the hash table
    m_symbol_manager_ptr->removeSymbolsFromHashTable(p_function.getSymbolTable());
}

void CodeGenerator::visit(CompoundStatementNode &p_compound_statement) {
    // Reconstruct the hash table for looking up the symbol entry
    m_symbol_manager_ptr->reconstructHashTableFromSymbolTable(
        p_compound_statement.getSymbolTable());

    if (is_main) {
        constexpr const char*const riscv_assembly_main_func_expr =
        ".section    .text\n"
        "    .align 2\n"
        "   .globl main         # emit symbol 'main' to the global symbol table\n"
        "   .type main, @function\n\n"
        
        "main:\n"
        "   # in the function prologue\n"
        "   addi sp, sp, -128   # move stack pointer to lower address to allocate a new stack\n"
        "   sw ra, 124(sp)      # save return address of the caller function in the current stack\n"
        "   sw s0, 120(sp)      # save frame pointer of the last stack in the current stack\n"
        "   addi s0, sp, 128    # move frame pointer to the bottom of the current stack\n\n";

        dumpInstructions(m_output_file.get(), riscv_assembly_main_func_expr);
        is_main = false;
    }

    if (is_if) {
        dumpInstructions(m_output_file.get(), "L%d:\n", current_label);
    }
    else if (is_else) {
        dumpInstructions(m_output_file.get(), "L%d:\n", current_label+1);
        is_else = false;
    }
    else if (is_while) {
        dumpInstructions(m_output_file.get(), "L%d:\n", current_label+1);
    }
    else if (is_for) {
        constexpr const char*const riscv_assembly_for_expr =
            "   lw t0, 0(sp)        # pop the value from the stack\n"
            "   addi sp, sp, 4\n"
            "   lw t1, 0(sp)        # pop the value from the stack\n"
            "   addi sp, sp, 4\n"
            "   bge t1, t0, L%d        # if i >= condition value, exit the loop\n"
            "L%d:\n";
        dumpInstructions(m_output_file.get(), riscv_assembly_for_expr, current_label+2, current_label+1);
    }

    p_compound_statement.visitChildNodes(*this);

    if (is_if) {
        constexpr const char*const riscv_assembly_if_expr =
            "   j L%d\n";
        dumpInstructions(m_output_file.get(), riscv_assembly_if_expr, current_label+2);
        is_if = false;
        is_else = true;
    }
    // Remove the entries in the hash table
    m_symbol_manager_ptr->removeSymbolsFromHashTable(
        p_compound_statement.getSymbolTable());
}

void CodeGenerator::visit(PrintNode &p_print) {
    dumpInstructions(m_output_file.get(), "\n# PrintNode\n");
    p_print.visitChildNodes(*this);
    constexpr const char*const riscv_assembly_print_expr = 
    "   lw a0, 0(sp)        # pop the value from the stack to the first argument register 'a0'\n"
    "   addi sp, sp, 4\n"
    "   jal ra, printInt    # call function 'printInt'\n";
    dumpInstructions(m_output_file.get(), riscv_assembly_print_expr);
    
}

void CodeGenerator::visit(BinaryOperatorNode &p_bin_op) {
    dumpInstructions(m_output_file.get(), "\n# BinaryOperatorNode %s\n", p_bin_op.getOpCString());
    p_bin_op.visitChildNodes(*this);
    constexpr const char*const riscv_assembly_bin_op_expr_loadreg =
        "   lw t0, 0(sp)        # pop the value from the stack\n"
        "   addi sp, sp, 4\n"
        "   lw t1, 0(sp)        # pop the value from the stack\n"
        "   addi sp, sp, 4\n";
    dumpInstructions(m_output_file.get(), riscv_assembly_bin_op_expr_loadreg);
    switch (p_bin_op.getOp()) {
        case Operator::kPlusOp:
            {
            constexpr const char*const riscv_assembly_bin_op_expr_add =
                "   add t0, t1, t0      # t0 = t1 + t0\n"
                "   addi sp, sp, -4\n"
                "   sw t0, 0(sp)        # push the value to the stack\n";
            dumpInstructions(m_output_file.get(), riscv_assembly_bin_op_expr_add);
            }
            break;
        case Operator::kMinusOp:
        {
            constexpr const char*const riscv_assembly_bin_op_expr_sub =
                "   sub t0, t1, t0      # t0 = t1 - t0\n"
                "   addi sp, sp, -4\n"
                "   sw t0, 0(sp)        # push the value to the stack\n";
            dumpInstructions(m_output_file.get(), riscv_assembly_bin_op_expr_sub);
        }
            break;
        case Operator::kMultiplyOp:
        {
            constexpr const char*const riscv_assembly_bin_op_expr_mul =
                "   mul t0, t1, t0      # t0 = t1 * t0\n"
                "   addi sp, sp, -4\n"
                "   sw t0, 0(sp)        # push the value to the stack\n";
            dumpInstructions(m_output_file.get(), riscv_assembly_bin_op_expr_mul);
        }
            break;
        case Operator::kDivideOp:
        {
            constexpr const char*const riscv_assembly_bin_op_expr_div =
                "   div t0, t1, t0      # t0 = t1 / t0\n"
                "   addi sp, sp, -4\n"
                "   sw t0, 0(sp)        # push the value to the stack\n";
            dumpInstructions(m_output_file.get(), riscv_assembly_bin_op_expr_div);
        }
            break;
        case Operator::kModOp:
        {
            constexpr const char*const riscv_assembly_bin_op_expr_mod =
                "   rem t0, t1, t0      # t0 = t1 % t0\n"
                "   addi sp, sp, -4\n"
                "   sw t0, 0(sp)        # push\n";
            dumpInstructions(m_output_file.get(), riscv_assembly_bin_op_expr_mod);
        }
            break;
        case Operator::kAndOp:
        {
            constexpr const char*const riscv_assembly_bin_op_expr_and =
                "   and t0, t1, t0      # t0 = t1 & t0\n"
                "   addi sp, sp, -4\n"
                "   sw t0, 0(sp)        # push the value to the stack\n";
            dumpInstructions(m_output_file.get(), riscv_assembly_bin_op_expr_and);
        }
            break;
        case Operator::kOrOp:
        {
            constexpr const char*const riscv_assembly_bin_op_expr_or =
                "   or t0, t1, t0      # t0 = t1 | t0\n"
                "   addi sp, sp, -4\n"
                "   sw t0, 0(sp)        # push the value to the stack\n";
            dumpInstructions(m_output_file.get(), riscv_assembly_bin_op_expr_or);
        }
            break;
        case Operator::kLessOp:
        {
            if (is_branch) {
                if (is_if) {
                    constexpr const char*const riscv_assembly_bin_op_expr_less =
                        "   bge t1, t0, L%d      # if t1 < t0, jump to L%d\n";
                    dumpInstructions(m_output_file.get(), riscv_assembly_bin_op_expr_less, current_label+1, current_label);
                }
                else if (is_while) {
                    constexpr const char*const riscv_assembly_bin_op_expr_less =
                        "   bge t1, t0, L%d      # if t1 < t0, jump to L%d\n";
                    dumpInstructions(m_output_file.get(), riscv_assembly_bin_op_expr_less, current_label+2, current_label);
                }
            }
            else {
                constexpr const char*const riscv_assembly_bin_op_expr_less =
                    "   slt t0, t1, t0      # t2 = t1 < t0\n"
                    "   addi sp, sp, -4\n"
                    "   sw t0, 0(sp)        # push the value to the stack\n";
                dumpInstructions(m_output_file.get(), riscv_assembly_bin_op_expr_less);
            }
        }
            break;
        case Operator::kLessOrEqualOp:
        {
            if (is_branch) {
                if (is_if) {
                    constexpr const char*const riscv_assembly_bin_op_expr_less_equal =
                        "   bgt t1, t0, L%d      # if t1 <= t0, jump to L%d\n";
                    dumpInstructions(m_output_file.get(), riscv_assembly_bin_op_expr_less_equal, current_label+1, current_label);
                }
                else if (is_while) {
                    constexpr const char*const riscv_assembly_bin_op_expr_less_equal =
                        "   bgt t1, t0, L%d      # if t1 <= t0, jump to L%d\n";
                    dumpInstructions(m_output_file.get(), riscv_assembly_bin_op_expr_less_equal, current_label+2, current_label);
                }
            }
            else {
                constexpr const char*const riscv_assembly_bin_op_expr_less_equal =
                    "   sle t0, t1, t0      # t2 = t1 <= t0\n"
                    "   addi sp, sp, -4\n"
                    "   sw t0, 0(sp)        # push the value to the stack\n";
                dumpInstructions(m_output_file.get(), riscv_assembly_bin_op_expr_less_equal);
            }
        }
            break;
        case Operator::kGreaterOp:
        {
            if (is_branch) {
                if (is_if) {
                    constexpr const char*const riscv_assembly_bin_op_expr_greater =
                        "   ble t1, t0, L%d      # if t1 > t0, jump to L%d\n";
                    dumpInstructions(m_output_file.get(), riscv_assembly_bin_op_expr_greater, current_label+1, current_label);
                }
                else if (is_while) {
                    constexpr const char*const riscv_assembly_bin_op_expr_greater =
                        "   ble t1, t0, L%d      # if t1 > t0, jump to L%d\n";
                    dumpInstructions(m_output_file.get(), riscv_assembly_bin_op_expr_greater, current_label+2, current_label);
                }
            }
            else {
                constexpr const char*const riscv_assembly_bin_op_expr_greater =
                    "   sgt t0, t1, t0      # t2 = t1 > t0\n"
                    "   addi sp, sp, -4\n"
                    "   sw t0, 0(sp)        # push the value to the stack\n";
                dumpInstructions(m_output_file.get(), riscv_assembly_bin_op_expr_greater);
            }
        }
            break;

        case Operator::kGreaterOrEqualOp:
        {
            if (is_branch) {
                if (is_if) {
                    constexpr const char*const riscv_assembly_bin_op_expr_greater_equal =
                        "   blt t1, t0, L%d      # if t1 >= t0, jump to L%d\n";
                    dumpInstructions(m_output_file.get(), riscv_assembly_bin_op_expr_greater_equal, current_label+1, current_label);
                }
                else if (is_while) {
                    constexpr const char*const riscv_assembly_bin_op_expr_greater_equal =
                        "   blt t1, t0, L%d      # if t1 >= t0, jump to L%d\n";
                    dumpInstructions(m_output_file.get(), riscv_assembly_bin_op_expr_greater_equal, current_label+2, current_label);
                }
            }
            else {
                constexpr const char*const riscv_assembly_bin_op_expr_greater_equal =
                    "   sge t0, t1, t0      # t2 = t1 >= t0\n"
                    "   addi sp, sp, -4\n"
                    "   sw t0, 0(sp)        # push the value to the stack\n";
                dumpInstructions(m_output_file.get(), riscv_assembly_bin_op_expr_greater_equal);
            }
        }
            break;

        case Operator::kEqualOp:
        {
            if (is_branch) {
                if (is_if) {
                    constexpr const char*const riscv_assembly_bin_op_expr_equal =
                        "   bne t1, t0, L%d      # if t1 == t0, jump to L%d\n";
                    dumpInstructions(m_output_file.get(), riscv_assembly_bin_op_expr_equal, current_label+1, current_label);
                }
                else if (is_while) {
                    constexpr const char*const riscv_assembly_bin_op_expr_equal =
                        "   bne t1, t0, L%d      # if t1 == t0, jump to L%d\n";
                    dumpInstructions(m_output_file.get(), riscv_assembly_bin_op_expr_equal, current_label+2, current_label);
                }
            }
            else {
                constexpr const char*const riscv_assembly_bin_op_expr_equal =
                    "   seq t0, t1, t0      # t2 = t1 == t0\n"
                    "   addi sp, sp, -4\n"
                    "   sw t0, 0(sp)        # push the value to the stack\n";
                dumpInstructions(m_output_file.get(), riscv_assembly_bin_op_expr_equal);
            }
        }
            break;

        case Operator::kNotEqualOp:
        {
            if (is_branch) {
                if (is_if) {
                    constexpr const char*const riscv_assembly_bin_op_expr_not_equal =
                        "   beq t1, t0, L%d      # if t1 != t0, jump to L%d\n";
                    dumpInstructions(m_output_file.get(), riscv_assembly_bin_op_expr_not_equal, current_label+1, current_label);
                }
                else if (is_while) {
                    constexpr const char*const riscv_assembly_bin_op_expr_not_equal =
                        "   beq t1, t0, L%d      # if t1 != t0, jump to L%d\n";
                    dumpInstructions(m_output_file.get(), riscv_assembly_bin_op_expr_not_equal, current_label+2, current_label);
                }
            }
            else {
                constexpr const char*const riscv_assembly_bin_op_expr_not_equal =
                    "   sne t0, t1, t0      # t2 = t1 != t0\n"
                    "   addi sp, sp, -4\n"
                    "   sw t0, 0(sp)        # push the value to the stack\n";
                dumpInstructions(m_output_file.get(), riscv_assembly_bin_op_expr_not_equal);
            }
        }
            break;
        
    }
    is_branch = false;
}

void CodeGenerator::visit(UnaryOperatorNode &p_un_op) {
    dumpInstructions(m_output_file.get(), "\n# UnaryOperatorNode\n");
    p_un_op.visitChildNodes(*this);
    constexpr const char*const riscv_assembly_un_op_expr =
        "   lw t0, 0(sp)        # pop the value from the stack\n"
        "   addi sp, sp, 4\n";
    dumpInstructions(m_output_file.get(), riscv_assembly_un_op_expr);
    switch (p_un_op.getOp()) {
        case Operator::kNegOp:
        {
            constexpr const char*const riscv_assembly_un_op_expr_neg =
                "   neg t0, t0          # t1 = -t0\n"
                "   addi sp, sp, -4\n"
                "   sw t0, 0(sp)        # push the value to the stack\n";
            dumpInstructions(m_output_file.get(), riscv_assembly_un_op_expr_neg);
        }
            break;
        case Operator::kNotOp:
        {
            constexpr const char*const riscv_assembly_un_op_expr_not =
                "   seq t0, t0, zero    # t1 = !t0\n"
                "   addi sp, sp, -4\n"
                "   sw t0, 0(sp)        # push the value to the stack\n";
            dumpInstructions(m_output_file.get(), riscv_assembly_un_op_expr_not);

            if (is_branch) {
                constexpr const char*const riscv_assembly_not_branch_expr=
                "   lw t1, 0(sp)         # pop the value from the stack\n"
                "   addi sp, sp, 4\n"
                "   li t0, 0\n"
                "   beq t1, t0, L%d      # if t1 == 0, jump to L%d\n";
                dumpInstructions(m_output_file.get(), riscv_assembly_not_branch_expr, current_label+1, current_label+1);
                is_branch = false;
            }
        }
            break;
    }
}

void CodeGenerator::visit(FunctionInvocationNode &p_func_invocation) {
    dumpInstructions(m_output_file.get(), "\n# FunctionInvocationNode\n");
    p_func_invocation.visitChildNodes(*this);
    constexpr const char*const riscv_assembly_func_invocation_callfromstack =
        "   lw a%d, 0(sp)\n"
        "   addi sp, sp, 4\n";
    for (int i= p_func_invocation.getArguments().size()-1; i>= 0; i--) {
        dumpInstructions(m_output_file.get(), riscv_assembly_func_invocation_callfromstack, i);
    }
    constexpr const char*const riscv_assembly_func_invocation_call =
        "   jal ra, %s\n"
        "   mv t0, a0\n"
        "   addi sp, sp, -4\n"
        "   sw t0, 0(sp)        # push the return value to the stack\n";
    dumpInstructions(m_output_file.get(), riscv_assembly_func_invocation_call, p_func_invocation.getNameCString());
}

void CodeGenerator::visit(VariableReferenceNode &p_variable_ref) {
    const SymbolEntry *symbol_entry = m_symbol_manager_ptr->lookup(p_variable_ref.getName());
    if (symbol_entry == nullptr) return;

    if(symbol_entry->getLevel() == 0){
        if (symbol_entry->getKind() == SymbolEntry::KindEnum::kVariableKind) {
            if(is_lvalue){
                is_lvalue = false;
                constexpr const char*const riscv_assembly_glval_ref_expr =
                    "   la t0, %s           # load the address of variable %s\n"
                    "   addi sp, sp, -4\n"
                    "   sw t0, 0(sp)     # push the address to the stack\n";
                dumpInstructions(m_output_file.get(), riscv_assembly_glval_ref_expr, p_variable_ref.getNameCString(), p_variable_ref.getNameCString());
            }
            else{
                constexpr const char*const riscv_assembly_grval_ref_expr =
                    "   la t0, %s\n"
                    "   lw t1, 0(t0)        # load the value of %s\n"
                    "   mv t0, t1\n"
                    "   addi sp, sp, -4\n"
                    "   sw t0, 0(sp)     # push the address to the stack\n";
                dumpInstructions(m_output_file.get(), riscv_assembly_grval_ref_expr, p_variable_ref.getNameCString(), p_variable_ref.getNameCString());
            }
        }
        else if (symbol_entry->getKind() == SymbolEntry::KindEnum::kConstantKind) {
            constexpr const char*const riscv_assembly_gconst_ref_expr =
                "   la t0, %s\n"
                "   lw t1, 0(t0)        # load the value of %s\n"
                "   mv t0, t1\n"
                "   addi sp, sp, -4\n"
                "   sw t0, 0(sp)     # push the value to the stack\n";
            dumpInstructions(m_output_file.get(), riscv_assembly_gconst_ref_expr, p_variable_ref.getNameCString(), p_variable_ref.getNameCString());
        }
    }

    // local variable, function parameter, loop variable
    else if(symbol_entry->getKind() == SymbolEntry::KindEnum::kVariableKind
        || symbol_entry->getKind() == SymbolEntry::KindEnum::kParameterKind
        || symbol_entry->getKind() == SymbolEntry::KindEnum::kLoopVarKind
        || symbol_entry->getKind() == SymbolEntry::KindEnum::kConstantKind){

        int addr = addr_stack[symbol_entry->getName()].top();
        // printf("load %s from stack %d\n", symbol_entry->getNameCString(), addr);
        if(is_lvalue){
            is_lvalue = false;
            constexpr const char*const riscv_assembly_llvalue_ref_expr =
            "   addi t0, s0, -%d\n"
            "   addi sp, sp, -4\n"
            "   sw t0, 0(sp)        # push the address to the stack\n";
            dumpInstructions(m_output_file.get(), riscv_assembly_llvalue_ref_expr, addr+4);
        }
        else{
            constexpr const char*const riscv_assembly_lrvalue_ref_expr =
            "   lw t0, -%d(s0)      # load the value of %s\n"
            "   addi sp, sp, -4\n"
            "   sw t0, 0(sp)        # push the value to the stack\n";
            dumpInstructions(m_output_file.get(), riscv_assembly_lrvalue_ref_expr, addr+4, p_variable_ref.getNameCString());
        }
    }
}

void CodeGenerator::visit(AssignmentNode &p_assignment) {
    dumpInstructions(m_output_file.get(), "\n# AssignmentNode\n");
    is_lvalue = true;
    p_assignment.visitChildNodes(*this);
    constexpr const char*const riscv_assembly_assign_expr =
    "   lw t0, 0(sp)        # pop the value from the stack\n"
    "   addi sp, sp, 4\n"
    "   lw t1, 0(sp)        # pop the address from the stack\n"
    "   addi sp, sp, 4\n"
    "   sw t0, 0(t1)        # save the value to %s\n";
    dumpInstructions(m_output_file.get(), riscv_assembly_assign_expr, p_assignment.getLvalue().getNameCString());

    if (is_for_head) {
        is_for_head = false;
        int addr = addr_stack[p_assignment.getLvalue().getName()].top();
        constexpr const char*const riscv_assembly_for_assign_expr=
            "L%d:\n"
            "   lw t0, -%d(s0)      # load the value of %s\n"
            "   addi sp, sp, -4\n"
            "   sw t0, 0(sp)        # push the value to the stack\n";
        dumpInstructions(m_output_file.get(), riscv_assembly_for_assign_expr, current_label, addr+4, p_assignment.getLvalue().getNameCString());
    }
}

void CodeGenerator::visit(ReadNode &p_read) {
    dumpInstructions(m_output_file.get(), "\n# ReadNode\n");
    is_lvalue = true;
    p_read.visitChildNodes(*this);
    constexpr const char*const riscv_assembly_read_expr =
        "   jal ra, readInt     # call function 'readInt'\n"
        "   lw t0, 0(sp)\n"
        "   addi sp, sp, 4\n"
        "   sw a0, 0(t0)        # push the value to the stack\n";
    dumpInstructions(m_output_file.get(), riscv_assembly_read_expr);
}

void CodeGenerator::visit(IfNode &p_if) {
    is_if = true;
    is_else = false;
    is_branch = true;
    label_base.push(label_num);
    label_num += 3;
    current_label = label_base.top();

    p_if.visitChildNodes(*this);
    is_if = false;
    is_else = false;
    dumpInstructions(m_output_file.get(), "L%d:\n", current_label+2);
    label_base.pop();
    if (!label_base.empty()) current_label = label_base.top();
}

void CodeGenerator::visit(WhileNode &p_while) {
    dumpInstructions(m_output_file.get(), "\n# WhileNode\n");
    label_base.push(label_num);
    label_num += 3;
    current_label = label_base.top();
    constexpr const char*const riscv_assembly_while_expr =
        "L%d:\n";
    dumpInstructions(m_output_file.get(), riscv_assembly_while_expr, current_label);
    is_while = true;
    is_branch = true;
    p_while.visitChildNodes(*this);
    is_while = false;
    constexpr const char*const riscv_assembly_while_expr2 =
        "   j L%d\n"
        "L%d:\n";
    dumpInstructions(m_output_file.get(), riscv_assembly_while_expr2, current_label, current_label+2);
    label_base.pop();
    if (!label_base.empty()) current_label = label_base.top();
}

void CodeGenerator::visit(ForNode &p_for) {
    // Reconstruct the hash table for looking up the symbol entry
    m_symbol_manager_ptr->reconstructHashTableFromSymbolTable(
        p_for.getSymbolTable());
    label_base.push(label_num);
    label_num += 3;
    current_label = label_base.top();
    is_for = true;
    is_for_head = true;
    p_for.visitChildNodes(*this);
    is_for = false;

    int addr = addr_stack[p_for.getLoopVarName()].top();
    constexpr const char*const riscv_assembly_for_expr =
        "   addi t0, s0, -%d      # load the address of '%s'\n"
        "   addi sp, sp, -4\n"
        "   sw t0, 0(sp)        # push the address to the stack\n"
        "   lw t0, -%d(s0)      # load the value of loop variable\n"
        "   addi sp, sp, -4\n"
        "   sw t0, 0(sp)        # push the value to the stack\n"
        "   li t0, 1\n"
        "   addi sp, sp, -4\n"
        "   sw t0, 0(sp)        # push the value to the stack\n"
        "   lw t0, 0(sp)        # pop the value from the stack\n"
        "   addi sp, sp, 4\n"
        "   lw t1, 0(sp)        # pop the value from the stack\n"
        "   addi sp, sp, 4\n"
        "   add t0, t1, t0      # %s + 1, always save the value in a certain register you choose\n"
        "   addi sp, sp, -4\n"
        "   sw t0, 0(sp)        # push the value to the stack\n"
        "   lw t0, 0(sp)        # pop the value from the stack\n"
        "   addi sp, sp, 4\n"
        "   lw t1, 0(sp)        # pop the address from the stack\n"
        "   addi sp, sp, 4\n"
        "   sw t0, 0(t1)        # save the value to loop variable\n"
        "   j L%d                # jump back to loop condition\n"
        "L%d:\n";
    dumpInstructions(m_output_file.get(), riscv_assembly_for_expr, addr+4, p_for.getLoopVarName(), addr+4, p_for.getLoopVarName(), current_label, current_label+2);
    label_base.pop();
    if (!label_base.empty()) current_label = label_base.top();
    // Remove the entries in the hash table
    m_symbol_manager_ptr->removeSymbolsFromHashTable(p_for.getSymbolTable());
}

void CodeGenerator::visit(ReturnNode &p_return) {
    p_return.visitChildNodes(*this);
    constexpr const char*const riscv_assembly_return_expr =
        "   lw t0, 0(sp)        # pop the value from the stack to 't0'\n"
        "   addi sp, sp, 4\n"
        "   mv a0, t0        # load the value to the return value register 'a0'\n";
    dumpInstructions(m_output_file.get(), riscv_assembly_return_expr);
}

void CodeGenerator::addrStackPush(const std::string p_name){
    if (addr_stack.find(p_name) == addr_stack.end()) {
        std::stack<int> addr;
        addr.push(local_addr);
        addr_stack[p_name] = addr;
    }
    else {
        addr_stack[p_name].push(local_addr);
    }
}

void CodeGenerator::addrStackPop(const std::string p_name){
    if (addr_stack.find(p_name) != addr_stack.end()) {
        if (!addr_stack[p_name].empty())
            addr_stack[p_name].pop();
        else {
            addr_stack.erase(p_name);
        }
    }
}