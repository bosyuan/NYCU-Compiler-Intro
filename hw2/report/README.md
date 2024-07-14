# hw2 report

|||
|-:|:-|
|Name|侯博軒|
|ID|110550167|

## How much time did you spend on this project

About 12 hours
 - 6 hours figuring out the requirements and learning YACC
 - 4.5 hours designing CFG for parsing and coding it up
 - 1.5 hours debugging and writting this report

## Project overview

This is a CFG parser divided into 2 parts. First, parse input text as tokens with a Lex program. Second, parse the tokens in order and examine if they fit the CFG standards.  The main focus of this assignment is designing the second part and writting the correct CFG. I addopted the released Lex program and only modified the part to return tokens to my parser. 

In the CFG parser are terminals(tokens) passed from Lex and my self defined unterminals. I listed the main unterminals in my program below:
1. Program: the start symbol of the parser
2. Function: declaration and definition of functions 
3. Variable/Constants: declaration of variables and constants
4. Statements: including all the operations like loop / condition / return ... etc
5. Expressions: simple calculating operations and comparison

With the help of several intermediate unterminals to express CFGs in a correect form, the above untermionals can describe correct P language programs. When it comes to implementation, YACC is more friendly to ambiguouse CFGs. Ambiguity on states such as all the possibilities of **expressions** may reach conflict if the CFG is not properly designed. Yet with the help of token precedence in YACC, there will be no need to derieve an unambiguous grammer with lots of intermediate states, making the process far more easier.


## What is the hardest you think in this project

The hardest part of this homework might be learning how to write YACC. I can not find much tutorials on how to learn YACC, and had to learn from reading other parser codes. At first I mistaken the task of this assignment as to derieve the parse tree of P language. I thus spent a lot of time working on understanding how to parse the input from lex using union structures. Hope this might come in handy in future assignments.
I also ran into some problems on deciding what "integer_constant" could be placed into the brackets of an array index. My final choice was choose to parse *array [ expression ]*. This rule was good enough to make me pass the testcase provided by TAs but I think this maybe troublesome. However, I haven't found a way to work pass this bug yet. 
## Feedback to T.A.s

Great homework! I learned a lot from writting my own YACC prgram from scratch. It gave me a different view on CFGs taught in class.
