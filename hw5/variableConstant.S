    .file "test/basic_cases/test-cases/variableConstant.p"
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
   li t0, 5            # load value to register 't0'
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
   la t0, gc
   lw t1, 0(t0)        # load the value of gc
   mv t0, t1
   addi sp, sp, -4
   sw t0, 0(sp)     # push the value to the stack
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

# PrintNode
   lw t0, -16(s0)      # load the value of lc
   addi sp, sp, -4
   sw t0, 0(sp)        # push the value to the stack
   lw a0, 0(sp)        # pop the value from the stack to the first argument register 'a0'
   addi sp, sp, 4
   jal ra, printInt    # call function 'printInt'
   # in the function epilogue
   lw ra, 124(sp)      # load return address saved in the current stack
   lw s0, 120(sp)      # move frame pointer back to the bottom of the last stack
   addi sp, sp, 128    # move stack pointer back to the top of the last stack
   jr ra               # jump back to the caller function
   .size main, .-main

