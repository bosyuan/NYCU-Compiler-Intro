# hw4 report

|||
|-:|:-|
|Name|侯博軒|
|ID|110550167|

## How much time did you spend on this project

> Total work time adds up to about 16 hours.
	Followign the steps in the instruction md, the time of implementing each semantic check was quite even, compared to previouse HWs.

## Project overview

> I built a semantic analyzer that can traverse all the nodes constructed in the AST, performing semantic check on each node.  A symbol table manager is constructed to hold all the symbol tables in a stack datatype, a symbol table is composed of declarations defined in the same scope.

>Sementic analysis is carried out in each node, performs sementic checking after treversing every child node. If a node an error is found in the current node, it returns to its caller(parent in a DFS graph) and acknowledge that it is faulty.  Faulty child nodes often occur in expression nodes, therefore, an implementation trick I used here is store a PType reference in every expression node. If the expression node is faulty the PType reference will be a nullptr, otherwise it will give the information on the type of the expression.

>The use of a symbol manager enablese look up in the symboltables when we need prior knowledge about the declared variable.  It also maintains the kind and scope of a variable, which is useful when checking return type or prohibiting loop variable atler insde a loop scope. 

>To sum up, using a symbol manager and semantic analyzer, sementic check is carried out in the process of constructing the AST bottom up. When error is detected, the error message is pushed into the error queue, and outputs the error message after printing out the symbol table, a special charecter D can be used to close printed symbol table output.

## What is the hardest you think in this project

> I think the hardest part of this project is debugging. 
It is time consuming to find segmentation falts when there are a lot of pointer refernces flying all around the place. I wished there were a debuger in the parser project that I can use to sort out these errors. 

>Overall, I think my implementation is quite messy and may be erroness. Sometimes it is quite confusing to decide whether to perform the error checking inside the node with a function defined inside the node class, or to pass childs of the node to the sementic analyzer then perform sementic checking. 

## Feedback to T.A.s

> Good HW, although it is sad to think about the upcomming HW5. 
>Too much compiler HW : (
