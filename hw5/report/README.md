# hw5 report

|||
|-:|:-|
|Name|侯博軒|
|ID|110550167|

## How much time did you spend on this project

> 15  hours.
= 3 hour code survey
+ 10 hour implementation
+ 2 hour code correction

## Project overview

>This is a code generator for the P language. Following the implementation of the AST tree from pass assignments, the code generator is designed as an agent that traverse the tree in preorder and produce the RISC-V assembly code. 
>Same as the works before, we have to design the visitor pattern traverse rule on different nodes to genertate codes for variables, expressions, loops, and conditions. The focus of code geneate on variables and expressions are dealing with registers and the address on the stack where these variables are stored. I designed a special data structure addr_stack to deal with variables in different scope, all the address of variables that are usable in current scope are stored in the top of addr_stack[variable name], and variables at the top of the stack are poped when leaving a scope. On the other hand, for actions such as conditioning and looping, the focus will be on correctly branching on unary/binary operations. To deal with nested loops and conditions, I store the L labels in a stack, similar to tackling varibles in different scope, once the condition or loop enters a different label condition, the label number will be pushed to the label stack and current label will be set as the top of the label stack. When leaving a scope, the label stack will be poped, and current label restored from the stack. 

## What is the hardest you think in this project

> I think the hardest part of this assignment is passing arguments to functions. I still couldn't figure out a way to pass arguments exceeding the argument register number 7. I don't know where the arguments could be stored and left unmodified on behalf of a function invocation. I tried storing arguments in the s registers, however, I ran into some segmentation faults executing my assembly code. This is the reason why I only got a score of 90/95 in the basic testcase. 

## Feedback to T.A.s

> Learned a lot from this course, by having hands on design experience of a compiler. Assignments are well designed, important concepts broken down to a semester long project work. This is a course that I will not regret taking even though it ment several sleepless nights battling through lines of complicated code.
