    .file "test/basic_cases/test-cases/function.p"
    .option nopic
#declare global variable
.comm gv, 4, 4    # emit object 'gv' to .bss section with size = 4, align = 4
#declare global constant
.section    .rodata
   .align 2
   .globl gc
   .type gc, @object
gc:
    .word 2
.section    .text
    .align 2
   .globl product         # emit symbol 'product' to the global symbol table
   .type product, @function

product:
   # in the function prologue
   addi sp, sp, -128   # move stack pointer to lower address to allocate a new stack
   sw ra, 124(sp)      # save return address of the caller function in the current stack
   sw s0, 120(sp)      # save frame pointer of the last stack in the current stack
   addi s0, sp, 128    # move frame pointer to the bottom of the current stack

   sw s0, -12(s0)      # save parameter a in the local stack
   sw s1, -16(s0)      # save parameter b in the local stack

# AssignmentNode
   addi t0, s0, -20
   addi sp, sp, -4
   sw t0, 0(sp)        # push the address to the stack

# BinaryOperatorNode
   lw t0, -12(s0)      # load the value of a
   addi sp, sp, -4
   sw t0, 0(sp)        # push the value to the stack
   lw t0, -16(s0)      # load the value of b
   addi sp, sp, -4
   sw t0, 0(sp)        # push the value to the stack
   lw t0, 0(sp)        # pop the value from the stack
   addi sp, sp, 4
   lw t1, 0(sp)        # pop the value from the stack
   addi sp, sp, 4
   mul t0, t1, t0      # t2 = t1 * t0
   addi sp, sp, -4
   sw t0, 0(sp)        # push the value to the stack
   lw t0, 0(sp)        # pop the value from the stack
   addi sp, sp, 4
   lw t1, 0(sp)        # pop the address from the stack
   addi sp, sp, 4
   sw t0, 0(t1)        # save the value to result
   lw t0, -20(s0)      # load the value of result
   addi sp, sp, -4
   sw t0, 0(sp)        # push the value to the stack
   lw t0, 0(sp)        # pop the value from the stack to 't0'
   addi sp, sp, 4
   mv a0, t0        # load the value to the return value register 'a0'
   # in the function epilogue
   lw ra, 124(sp)      # load return address saved in the current stack
   lw s0, 120(sp)      # move frame pointer back to the bottom of the last stack
   addi sp, sp, 128    # move stack pointer back to the top of the last stack
   jr ra               # jump back to the caller function
   .size product, .-product

.section    .text
    .align 2
   .globl sum         # emit symbol 'sum' to the global symbol table
   .type sum, @function

sum:
   # in the function prologue
   addi sp, sp, -128   # move stack pointer to lower address to allocate a new stack
   sw ra, 124(sp)      # save return address of the caller function in the current stack
   sw s0, 120(sp)      # save frame pointer of the last stack in the current stack
   addi s0, sp, 128    # move frame pointer to the bottom of the current stack

   sw s0, -12(s0)      # save parameter a in the local stack
   sw s1, -16(s0)      # save parameter b in the local stack

# AssignmentNode
   addi t0, s0, -20
   addi sp, sp, -4
   sw t0, 0(sp)        # push the address to the stack

# BinaryOperatorNode
   lw t0, -12(s0)      # load the value of a
   addi sp, sp, -4
   sw t0, 0(sp)        # push the value to the stack
   lw t0, -16(s0)      # load the value of b
   addi sp, sp, -4
   sw t0, 0(sp)        # push the value to the stack
   lw t0, 0(sp)        # pop the value from the stack
   addi sp, sp, 4
   lw t1, 0(sp)        # pop the value from the stack
   addi sp, sp, 4
   add t0, t1, t0      # t2 = t1 + t0
   addi sp, sp, -4
   sw t0, 0(sp)        # push the value to the stack
   lw t0, 0(sp)        # pop the value from the stack
   addi sp, sp, 4
   lw t1, 0(sp)        # pop the address from the stack
   addi sp, sp, 4
   sw t0, 0(t1)        # save the value to result
   lw t0, -20(s0)      # load the value of result
   addi sp, sp, -4
   sw t0, 0(sp)        # push the value to the stack
   lw t0, 0(sp)        # pop the value from the stack to 't0'
   addi sp, sp, 4
   mv a0, t0        # load the value to the return value register 'a0'
   # in the function epilogue
   lw ra, 124(sp)      # load return address saved in the current stack
   lw s0, 120(sp)      # move frame pointer back to the bottom of the last stack
   addi sp, sp, 128    # move stack pointer back to the top of the last stack
   jr ra               # jump back to the caller function
   .size sum, .-sum

.section    .text
    .align 2
   .globl dot         # emit symbol 'dot' to the global symbol table
   .type dot, @function

dot:
   # in the function prologue
   addi sp, sp, -128   # move stack pointer to lower address to allocate a new stack
   sw ra, 124(sp)      # save return address of the caller function in the current stack
   sw s0, 120(sp)      # save frame pointer of the last stack in the current stack
   addi s0, sp, 128    # move frame pointer to the bottom of the current stack

   sw s0, -12(s0)      # save parameter x1 in the local stack
   sw s1, -16(s0)      # save parameter y1 in the local stack
   sw s2, -20(s0)      # save parameter x2 in the local stack
   sw s3, -24(s0)      # save parameter y2 in the local stack

# AssignmentNode
   addi t0, s0, -28
   addi sp, sp, -4
   sw t0, 0(sp)        # push the address to the stack

# FunctionInvocationNode

# FunctionInvocationNode
   lw t0, -12(s0)      # load the value of x1
   addi sp, sp, -4
   sw t0, 0(sp)        # push the value to the stack
   lw t0, -16(s0)      # load the value of y1
   addi sp, sp, -4
   sw t0, 0(sp)        # push the value to the stack
   lw s1, 0(sp)
   addi sp, sp, 4
   lw s0, 0(sp)
   addi sp, sp, 4
   jal ra, product
   mv t0, a0
   addi sp, sp, -4
   sw t0, 0(sp)        # push the return value to the stack

# FunctionInvocationNode
   lw t0, -20(s0)      # load the value of x2
   addi sp, sp, -4
   sw t0, 0(sp)        # push the value to the stack
   lw t0, -24(s0)      # load the value of y2
   addi sp, sp, -4
   sw t0, 0(sp)        # push the value to the stack
   lw s1, 0(sp)
   addi sp, sp, 4
   lw s0, 0(sp)
   addi sp, sp, 4
   jal ra, product
   mv t0, a0
   addi sp, sp, -4
   sw t0, 0(sp)        # push the return value to the stack
   lw s1, 0(sp)
   addi sp, sp, 4
   lw s0, 0(sp)
   addi sp, sp, 4
   jal ra, sum
   mv t0, a0
   addi sp, sp, -4
   sw t0, 0(sp)        # push the return value to the stack
   lw t0, 0(sp)        # pop the value from the stack
   addi sp, sp, 4
   lw t1, 0(sp)        # pop the address from the stack
   addi sp, sp, 4
   sw t0, 0(t1)        # save the value to result
   lw t0, -28(s0)      # load the value of result
   addi sp, sp, -4
   sw t0, 0(sp)        # push the value to the stack
   lw t0, 0(sp)        # pop the value from the stack to 't0'
   addi sp, sp, 4
   mv a0, t0        # load the value to the return value register 'a0'
   # in the function epilogue
   lw ra, 124(sp)      # load return address saved in the current stack
   lw s0, 120(sp)      # move frame pointer back to the bottom of the last stack
   addi sp, sp, 128    # move stack pointer back to the top of the last stack
   jr ra               # jump back to the caller function
   .size dot, .-dot

.section    .text
    .align 2
   .globl main         # emit symbol 'main' to the global symbol table
   .type main, @function

main:
   # in the function prologue
   addi sp, sp, -128   # move stack pointer to lower address to allocate a new stack
   sw ra, 124(sp)      # save return address of the caller function in the current stack
   sw s0, 120(sp)      # save frame pointer of the last stack in the current stack
   addi s0, sp, 128    # move frame pointer to the bottom of the current stack

#declare local constant
   addi t0, s0, -16
   addi sp, sp, -4
   sw t0, 0(sp)        # push the address to the stack
   li t0, 2            # load value to register 't0'
   addi sp, sp, -4
   sw t0, 0(sp)        # push the value to the stack
   lw t0, 0(sp)        # pop the value from the stack
   addi sp, sp, 4
   lw t1, 0(sp)        # pop the address from the stack
   addi sp, sp, 4
   sw t0, 0(t1)

# AssignmentNode
   la t0, gv           # load the address of variable gv
   addi sp, sp, -4
   sw t0, 0(sp)     # push the address to the stack
   li t0, 2            # load value to register 't0'
   addi sp, sp, -4
   sw t0, 0(sp)        # push the value to the stack
   lw t0, 0(sp)        # pop the value from the stack
   addi sp, sp, 4
   lw t1, 0(sp)        # pop the address from the stack
   addi sp, sp, 4
   sw t0, 0(t1)        # save the value to gv

# AssignmentNode
   addi t0, s0, -12
   addi sp, sp, -4
   sw t0, 0(sp)        # push the address to the stack
   li t0, 2            # load value to register 't0'
   addi sp, sp, -4
   sw t0, 0(sp)        # push the value to the stack
   lw t0, 0(sp)        # pop the value from the stack
   addi sp, sp, 4
   lw t1, 0(sp)        # pop the address from the stack
   addi sp, sp, 4
   sw t0, 0(t1)        # save the value to lv

# AssignmentNode
   la t0, gv           # load the address of variable gv
   addi sp, sp, -4
   sw t0, 0(sp)     # push the address to the stack

# FunctionInvocationNode
   la t0, gv
   lw t1, 0(t0)        # load the value of gv
   mv t0, t1
   addi sp, sp, -4
   sw t0, 0(sp)     # push the address to the stack
   la t0, gc
   lw t1, 0(t0)        # load the value of gc
   mv t0, t1
   addi sp, sp, -4
   sw t0, 0(sp)     # push the value to the stack
   lw s1, 0(sp)
   addi sp, sp, 4
   lw s0, 0(sp)
   addi sp, sp, 4
   jal ra, product
   mv t0, a0
   addi sp, sp, -4
   sw t0, 0(sp)        # push the return value to the stack
   lw t0, 0(sp)        # pop the value from the stack
   addi sp, sp, 4
   lw t1, 0(sp)        # pop the address from the stack
   addi sp, sp, 4
   sw t0, 0(t1)        # save the value to gv

# AssignmentNode
   addi t0, s0, -12
   addi sp, sp, -4
   sw t0, 0(sp)        # push the address to the stack

# BinaryOperatorNode
   la t0, gv
   lw t1, 0(t0)        # load the value of gv
   mv t0, t1
   addi sp, sp, -4
   sw t0, 0(sp)     # push the address to the stack

# FunctionInvocationNode
   lw t0, -12(s0)      # load the value of lv
   addi sp, sp, -4
   sw t0, 0(sp)        # push the value to the stack
   lw t0, -16(s0)      # load the value of lc
   addi sp, sp, -4
   sw t0, 0(sp)        # push the value to the stack
   lw s1, 0(sp)
   addi sp, sp, 4
   lw s0, 0(sp)
   addi sp, sp, 4
   jal ra, product
   mv t0, a0
   addi sp, sp, -4
   sw t0, 0(sp)        # push the return value to the stack
   lw t0, 0(sp)        # pop the value from the stack
   addi sp, sp, 4
   lw t1, 0(sp)        # pop the value from the stack
   addi sp, sp, 4
   add t0, t1, t0      # t2 = t1 + t0
   addi sp, sp, -4
   sw t0, 0(sp)        # push the value to the stack
   lw t0, 0(sp)        # pop the value from the stack
   addi sp, sp, 4
   lw t1, 0(sp)        # pop the address from the stack
   addi sp, sp, 4
   sw t0, 0(t1)        # save the value to lv

# PrintNode
   la t0, gv
   lw t1, 0(t0)        # load the value of gv
   mv t0, t1
   addi sp, sp, -4
   sw t0, 0(sp)     # push the address to the stack
   lw a0, 0(sp)        # pop the value from the stack to the first argument register 'a0'
   addi sp, sp, 4
   jal ra, printInt    # call function 'printInt'

# PrintNode
   lw t0, -12(s0)      # load the value of lv
   addi sp, sp, -4
   sw t0, 0(sp)        # push the value to the stack
   lw a0, 0(sp)        # pop the value from the stack to the first argument register 'a0'
   addi sp, sp, 4
   jal ra, printInt    # call function 'printInt'

# AssignmentNode
   la t0, gv           # load the address of variable gv
   addi sp, sp, -4
   sw t0, 0(sp)     # push the address to the stack

# FunctionInvocationNode
   la t0, gv
   lw t1, 0(t0)        # load the value of gv
   mv t0, t1
   addi sp, sp, -4
   sw t0, 0(sp)     # push the address to the stack
   la t0, gc
   lw t1, 0(t0)        # load the value of gc
   mv t0, t1
   addi sp, sp, -4
   sw t0, 0(sp)     # push the value to the stack
   lw t0, -12(s0)      # load the value of lv
   addi sp, sp, -4
   sw t0, 0(sp)        # push the value to the stack
   lw t0, -16(s0)      # load the value of lc
   addi sp, sp, -4
   sw t0, 0(sp)        # push the value to the stack
   lw s3, 0(sp)
   addi sp, sp, 4
   lw s2, 0(sp)
   addi sp, sp, 4
   lw s1, 0(sp)
   addi sp, sp, 4
   lw s0, 0(sp)
   addi sp, sp, 4
   jal ra, dot
   mv t0, a0
   addi sp, sp, -4
   sw t0, 0(sp)        # push the return value to the stack
   lw t0, 0(sp)        # pop the value from the stack
   addi sp, sp, 4
   lw t1, 0(sp)        # pop the address from the stack
   addi sp, sp, 4
   sw t0, 0(t1)        # save the value to gv

# PrintNode
   la t0, gv
   lw t1, 0(t0)        # load the value of gv
   mv t0, t1
   addi sp, sp, -4
   sw t0, 0(sp)     # push the address to the stack
   lw a0, 0(sp)        # pop the value from the stack to the first argument register 'a0'
   addi sp, sp, 4
   jal ra, printInt    # call function 'printInt'
   # in the function epilogue
   lw ra, 124(sp)      # load return address saved in the current stack
   lw s0, 120(sp)      # move frame pointer back to the bottom of the last stack
   addi sp, sp, 128    # move stack pointer back to the top of the last stack
   jr ra               # jump back to the caller function
   .size main, .-main

