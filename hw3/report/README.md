
# hw3 report

|||
|-:|:-|
|Name|侯博軒|
|ID|110550167|

## How much time did you spend on this project

> 24 hours.
> Spent a lot of time understanding the structure of the parser template, the implementation was also quite time consuming, the total lines I edited adds up to approximately 2k : (
## Project overview

>I employed the Visitor Pattern (VP) structure provided in the template. By constructing the AST for the input 'p' language, the parser is now capable of performing semantic analyses. When tested on the cases provided by the TAs, I ensured that the traversal of the AST could effectively break down the semantic meaning of the input language.

>The AST is built bottom-up in conjunction with Context-Free Grammar (CFG) parsing of the input. Nodes are constructed using different classes based on the specific semantic attributes of each node. For instance, there are program nodes, declaration nodes, assignment nodes, and so on. However, all these nodes inherit a common parent class: `AstNode`. This inheritance relationship aids in building the visiting action in the Visitor Pattern process. The visitor does not need to know the specific type of the node; instead, it asserts that all nodes are `AstNodes` and requests output information from these nodes. Therefore, it is the nodes' responsibility to pass internal information to the visiting visitors. After receiving the information, the dumper performs a pre-order printing of nodes.

>In summary, the parser constructs an AST during the parsing action, and a visitor, named `ATSDumper`, is capable of traversing and printing the information within the nodes.


## What is the hardest you think in this project

>I find this assignment significantly more challenging than previous homework assignments. Understanding how to build the Abstract Syntax Tree (AST) during parsing has proven to be quite complex, not to mention the substantial amount of code modifications required to construct a complete AST.

>I was relieved when the Teaching Assistants provided hints on implementing the Visitor Pattern (VP); it saved me a considerable amount of trouble, especially in avoiding the need to handle the printing process independently. Additionally, I am grateful for the well-designed test cases. Following the sequence of test cases helped me effectively test the newly added AST building process. It's difficult to imagine debugging on such a massive project without the aid of unit test cases.

## Feedback to T.A.s

> Great homework, took me a lot of time to finish, yet also learned a lot.
