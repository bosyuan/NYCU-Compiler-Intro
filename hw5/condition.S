    .file "test/basic_cases/test-cases/condition.p"
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
   .globl sum         # emit symbol 'sum' to the global symbol table
   .type sum, @function

sum:
   # in the function prologue
   addi sp, sp, -128   # move stack pointer to lower address to allocate a new stack
   sw ra, 124(sp)      # save return address of the caller function in the current stack
   sw s0, 120(sp)      # save frame pointer of the last stack in the current stack
   addi s0, sp, 128    # move frame pointer to the bottom of the current stack

   sw a0, -12(s0)      # save parameter a in the local stack
   sw a1, -16(s0)      # save parameter b in the local stack

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
   sw t0, 0(t1)        # save the value to c
   lw t0, -20(s0)      # load the value of c
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
   .globl main         # emit symbol 'main' to the global symbol table
   .type main, @function

main:
   # in the function prologue
   addi sp, sp, -128   # move stack pointer to lower address to allocate a new stack
   sw ra, 124(sp)      # save return address of the caller function in the current stack
   sw s0, 120(sp)      # save frame pointer of the last stack in the current stack
   addi s0, sp, 128    # move frame pointer to the bottom of the current stack


# AssignmentNode
   la t0, gv           # load the address of variable gv
   addi sp, sp, -4
   sw t0, 0(sp)     # push the address to the stack
   li t0, 1            # load value to register 't0'
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
   li t0, 3            # load value to register 't0'
   addi sp, sp, -4
   sw t0, 0(sp)        # push the value to the stack
   lw t0, 0(sp)        # pop the value from the stack
   addi sp, sp, 4
   lw t1, 0(sp)        # pop the address from the stack
   addi sp, sp, 4
   sw t0, 0(t1)        # save the value to lv

# BinaryOperatorNode
   la t0, gv
   lw t1, 0(t0)        # load the value of gv
   mv t0, t1
   addi sp, sp, -4
   sw t0, 0(sp)     # push the address to the stack
   li t0, 1            # load value to register 't0'
   addi sp, sp, -4
   sw t0, 0(sp)        # push the value to the stack
   lw t0, 0(sp)        # pop the value from the stack
   addi sp, sp, 4
   lw t1, 0(sp)        # pop the value from the stack
   addi sp, sp, 4
   bne t1, t0, L1      # if t1 == t0, jump to L0
   j L0

# PrintNode
   la t0, gv
   lw t1, 0(t0)        # load the value of gv
   mv t0, t1
   addi sp, sp, -4
   sw t0, 0(sp)     # push the address to the stack
   lw a0, 0(sp)        # pop the value from the stack to the first argument register 'a0'
   addi sp, sp, 4
   jal ra, printInt    # call function 'printInt'
L1:

# PrintNode
   lw t0, -12(s0)      # load the value of lv
   addi sp, sp, -4
   sw t0, 0(sp)        # push the value to the stack
   lw a0, 0(sp)        # pop the value from the stack to the first argument register 'a0'
   addi sp, sp, 4
   jal ra, printInt    # call function 'printInt'
L2:

# BinaryOperatorNode

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
   lw a1, 0(sp)
   addi sp, sp, 4
   lw a0, 0(sp)
   addi sp, sp, 4
   jal ra, sum
   mv t0, a0
   addi sp, sp, -4
   sw t0, 0(sp)        # push the return value to the stack
   li t0, 4            # load value to register 't0'
   addi sp, sp, -4
   sw t0, 0(sp)        # push the value to the stack
   lw t0, 0(sp)        # pop the value from the stack
   addi sp, sp, 4
   lw t1, 0(sp)        # pop the value from the stack
   addi sp, sp, 4
   ble t1, t0, L4      # if t1 > t0, jump to L3
   j L3

# PrintNode
   la t0, gv
   lw t1, 0(t0)        # load the value of gv
   mv t0, t1
   addi sp, sp, -4
   sw t0, 0(sp)     # push the address to the stack
   lw a0, 0(sp)        # pop the value from the stack to the first argument register 'a0'
   addi sp, sp, 4
   jal ra, printInt    # call function 'printInt'
L4:

# PrintNode
   lw t0, -12(s0)      # load the value of lv
   addi sp, sp, -4
   sw t0, 0(sp)        # push the value to the stack
   lw a0, 0(sp)        # pop the value from the stack to the first argument register 'a0'
   addi sp, sp, 4
   jal ra, printInt    # call function 'printInt'
L5:
   # in the function epilogue
   lw ra, 124(sp)      # load return address saved in the current stack
   lw s0, 120(sp)      # move frame pointer back to the bottom of the last stack
   addi sp, sp, 128    # move stack pointer back to the top of the last stack
   jr ra               # jump back to the caller function
   .size main, .-main

