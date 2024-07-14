%{
#include "AST/BinaryOperator.hpp"
#include "AST/CompoundStatement.hpp"
#include "AST/ConstantValue.hpp"
#include "AST/FunctionInvocation.hpp"
#include "AST/UnaryOperator.hpp"
#include "AST/VariableReference.hpp"
#include "AST/assignment.hpp"
#include "AST/ast.hpp"
#include "AST/decl.hpp"
#include "AST/expression.hpp"
#include "AST/for.hpp"
#include "AST/function.hpp"
#include "AST/if.hpp"
#include "AST/print.hpp"
#include "AST/program.hpp"
#include "AST/read.hpp"
#include "AST/return.hpp"
#include "AST/variable.hpp"
#include "AST/while.hpp"
#include "AST/AstDumper.hpp"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#define YYLTYPE yyltype

typedef struct YYLTYPE {
    uint32_t first_line;
    uint32_t first_column;
    uint32_t last_line;
    uint32_t last_column;
} yyltype;

extern uint32_t line_num;   /* declared in scanner.l */
extern char current_line[]; /* declared in scanner.l */
extern FILE *yyin;          /* declared by lex */
extern char *yytext;        /* declared by lex */

static AstNode *root;

extern "C" int yylex(void);
static void yyerror(const char *msg);
extern int yylex_destroy(void);

// internal global function
bool is_bool = false;
Ptype *constant_type;
std::vector<int> *array_size = nullptr;
%}
%code requires {
    class AstNode;
    class ProgramNode;
    class DeclNode;
    class VariableNode;
    class ConstantValueNode;
    class FunctionNode;
    class CompoundStatementNode;
    class PrintNode;
    class BinaryOperatorNode;
    class UnaryOperatorNode;
    class FunctionInvocationNode;
    class VariableReferenceNode;
    class AssignmentNode;
    class ReadNode;
    class IfNode;
    class WhileNode;
    class ForNode;
    class ReturnNode;
    class ExpressionNode;

    #include <vector>
    #include <string>
    #include "AST/utils.hpp"
}

    /* For yylval */
%union {
    /* basic semantic value */
    int int_val;
    float float_val;
    char* string_val;
    char *identifier;
    bool bool_val;

    AstNode *node;
    std::vector<AstNode *> *ats_list;
    
    ConstantValueNode *constant_value_node;
    CompoundStatementNode *compound_statement_node;

    DeclNode *declaration_node;
    std::vector<DeclNode* > *declaration_list;

    ExpressionNode *expression_node;
    std::vector<ExpressionNode *> *expression_list;

    FunctionNode *function_node;
    std::vector<FunctionNode* > *function_list;
    FunctionInvocationNode *function_invocation_node;
    ForNode *for_node;

    IfNode *if_node;

    ProgramNode *program_node;
    Ptype *pytpe_ptr;

    std::vector<VariableNode *> *id_list;
    std::string *string_ptr;
    ReturnNode *return_node;
    VariableReferenceNode *variable_reference_node;

    WhileNode *while_node;

};

%type <identifier> ProgramName ID FunctionName
%type <node> ProgramUnit Statement Simple
%type <ats_list> StatementList Statements
%type <program_node> Program
%type <declaration_list> DeclarationList Declarations FormalArgs FormalArgList
%type <declaration_node> Declaration FormalArg
%type <function_list> FunctionList Functions
%type <function_node> Function FunctionDeclaration FunctionDefinition
%type <compound_statement_node> CompoundStatement ElseOrNot
%type <pytpe_ptr> ScalarType Type ArrType ReturnType
%type <node> ArrDecl;
%type <id_list> IdList
%type <constant_value_node> LiteralConstant IntegerAndReal 
%type <bool_val> NegOrNot
%type <string_ptr> StringAndBoolean
%type <expression_node> Expression
%type <expression_list> ExpressionList Expressions ArrRefs ArrRefList
%type <function_invocation_node> FunctionInvocation
%type <variable_reference_node> VariableReference
%type <if_node> Condition
%type <while_node> While
%type <for_node> For
%type <return_node> Return
    /* Follow the order in scanner.l */

    /* Delimiter */
%token COMMA SEMICOLON COLON
%token L_PARENTHESIS R_PARENTHESIS
%token L_BRACKET R_BRACKET

    /* Operator */
%token ASSIGN
%left OR
%left AND
%right NOT
%left LESS LESS_OR_EQUAL EQUAL GREATER GREATER_OR_EQUAL NOT_EQUAL
%left PLUS MINUS
%left MULTIPLY DIVIDE MOD
%right UNARY_MINUS

    /* Keyword */
%token ARRAY BOOLEAN INTEGER REAL STRING
%token END BEGIN_ /* Use BEGIN_ since BEGIN is a keyword in lex */
%token DO ELSE FOR IF THEN WHILE
%token DEF OF TO RETURN VAR
%token FALSE TRUE
%token PRINT READ

    /* Identifier */
%token ID

    /* Literal */
%token <int_val> INT_LITERAL
%token <float_val> REAL_LITERAL
%token <string_val> STRING_LITERAL

%%

ProgramUnit:
    Program
    |
    Function
;

Program:
    ProgramName SEMICOLON
    /* ProgramBody */
    DeclarationList FunctionList CompoundStatement
    /* End of ProgramBody */
    END {
        root = new ProgramNode(@1.first_line, @1.first_column,
                               $1, $3, $4, $5);
        
        free($1);
    }
;

ProgramName:
    ID {
        $$ = $1;
    }
;

DeclarationList:
    Epsilon {
        $$ = nullptr;
    }
    |
    Declarations {
        $$ = $1;
    }
;

Declarations:
    Declaration {
        $$ = new std::vector<DeclNode *>();
        $$->push_back($1);
    }
    |
    Declarations Declaration {
        $1->push_back($2);
        $$ = $1;
    }
;

FunctionList:
    Epsilon {
        $$ = nullptr;
    }
    |
    Functions {
        $$ = $1;
    }
;

Functions:
    Function {
        $$ = new std::vector<FunctionNode *>();
        $$->push_back($1);
    }
    |
    Functions Function {
        $1->push_back($2);
        $$ = $1;
    }
;

Function:
    FunctionDeclaration 
    |
    FunctionDefinition
;

FunctionDeclaration:
    FunctionName L_PARENTHESIS FormalArgList R_PARENTHESIS ReturnType SEMICOLON {
        $$ = new FunctionNode(@1.first_line, @1.first_column,
                              $1, $3, $5, nullptr);
    }
;

FunctionDefinition:
    FunctionName L_PARENTHESIS FormalArgList R_PARENTHESIS ReturnType
    CompoundStatement
    END {
        $$ = new FunctionNode(@1.first_line, @1.first_column,
                              $1, $3, $5, $6);
    }
;

FunctionName:
    ID {
        $$ = $1;
    }
;

FormalArgList:
    Epsilon {
        $$ = nullptr;
    }
    |
    FormalArgs {
        $$ = $1;
    }
;

FormalArgs:
    FormalArg {
        $$ = new std::vector<DeclNode *>();
        $$->push_back($1);
    }
    |
    FormalArgs SEMICOLON FormalArg {
        $1->push_back($3);
        $$ = $1;
    }
;

FormalArg:
    IdList COLON Type {
        $$ = new DeclNode(@1.first_line, @1.first_column,
                          $1, $3, array_size);
        delete array_size;
        array_size = nullptr;
    }
;

IdList:
    ID {
        $$ = new std::vector<VariableNode *>();
        $$->push_back(new VariableNode(@1.first_line, @1.first_column, $1));
    }
    |
    IdList COMMA ID {
        $1->push_back(new VariableNode(@3.first_line, @3.first_column, $3));
        $$ = $1;
    }
;

ReturnType:
    COLON ScalarType {
        $$ = $2;
    }
    |
    Epsilon {
        $$ = new Ptype(Ptype::PVoid);
    }
;

    /*
       Data Types and Declarations
                                   */

Declaration:
    VAR IdList COLON Type SEMICOLON {
        $$ = new DeclNode(@1.first_line, @1.first_column,
                          $2, $4, array_size);
        delete array_size;
        array_size = nullptr;
    }
    |
    VAR IdList COLON LiteralConstant SEMICOLON {
        $$ = new DeclNode(@1.first_line, @1.first_column,
                          $2, constant_type, $4);
        delete constant_type;
    }
;

Type:
    ScalarType {
        $$ = $1;
    }
    |
    ArrType {
        $$ = $1;
    }
;

ScalarType:
    INTEGER {
        $$ = new Ptype(Ptype::PInt);
    }
    |
    REAL {
        $$ = new Ptype(Ptype::PReal);
    }
    |
    STRING {
        $$ = new Ptype(Ptype::PString);
    }
    |
    BOOLEAN {
        $$ = new Ptype(Ptype::PBoolean);
    }
;

ArrType:
    ArrDecl ScalarType {
        $$ = $2;
    }
;

ArrDecl:
    ARRAY INT_LITERAL OF {
        array_size = new std::vector<int>();
        array_size->push_back($2);
        $$ = nullptr;
    }
    |
    ArrDecl ARRAY INT_LITERAL OF {
        array_size->push_back($3);
        $$ = nullptr;
    }
;

LiteralConstant:
    NegOrNot INT_LITERAL {
        constant_type = new Ptype(Ptype::PInt);
        if ($1) {
            $$ = new ConstantValueNode(@1.first_line, @1.first_column, -$2);
        } else {
            $$ = new ConstantValueNode(@2.first_line, @2.first_column, $2);
        }
    }
    |
    NegOrNot REAL_LITERAL {
        constant_type = new Ptype(Ptype::PReal);
        if ($1) {
            $$ = new ConstantValueNode(@1.first_line, @1.first_column, -$2);
        } else {
            $$ = new ConstantValueNode(@2.first_line, @2.first_column, $2);
        }
    }
    |
    StringAndBoolean {
        if (is_bool) {
            constant_type = new Ptype(Ptype::PBoolean);
            $$ = new ConstantValueNode(@1.first_line, @1.first_column, $1);
            is_bool = false;
        } else {
            constant_type = new Ptype(Ptype::PString);
            $$ = new ConstantValueNode(@1.first_line, @1.first_column, $1);
        }
        is_bool = false;
    }
;

NegOrNot:
    Epsilon {
        $$ = false;
    }
    |
    MINUS %prec UNARY_MINUS {
        $$ = true;
    }
;

StringAndBoolean:
    STRING_LITERAL {
        $$ = new std::string( $1 );
    }
    |
    TRUE {
        $$ = new std::string("true");
        is_bool = true;
    }
    |
    FALSE {
        $$ = new std::string("false");
        is_bool = true;
    }
;

IntegerAndReal:
    INT_LITERAL {
        $$ = new ConstantValueNode(@1.first_line, @1.first_column, $1);
    }
    |
    REAL_LITERAL {
        $$ = new ConstantValueNode(@1.first_line, @1.first_column, $1);
    }
;

    /*
       Statements
                  */

Statement:
    CompoundStatement {
        $$ = static_cast<AstNode *>($1);
    }
    |
    Simple {
        $$ = $1;
    }
    |
    Condition {
        $$ = static_cast<AstNode *>($1);
    }
    |
    While {
        $$ = static_cast<AstNode *>($1);
    }
    |
    For {
        $$ = static_cast<AstNode *>($1);
    }
    |
    Return {
        $$ = static_cast<AstNode *>($1);
    }
    |
    FunctionCall
;

CompoundStatement:
    BEGIN_
    DeclarationList
    StatementList
    END {
        $$ = new CompoundStatementNode(@1.first_line, @1.first_column, $2, $3);
    }
;

Simple:
    VariableReference ASSIGN Expression SEMICOLON {
        $$ = static_cast<AstNode*>(new AssignmentNode(@2.first_line, @2.first_column, $1, $3));
    }
    |
    PRINT Expression SEMICOLON {
        $$ = static_cast<AstNode*>(new PrintNode(@1.first_line, @1.first_column, $2));
    }
    |
    READ VariableReference SEMICOLON {
        $$ = static_cast<AstNode*>(new ReadNode(@1.first_line, @1.first_column, $2));
    }
;

VariableReference:
    ID ArrRefList {
        $$ = new VariableReferenceNode(@1.first_line, @1.first_column, $1, $2);
    }
;

ArrRefList:
    Epsilon {
        $$ = nullptr;
    }
    |
    ArrRefs {
        $$ = $1;
    }
;

ArrRefs:
    L_BRACKET Expression R_BRACKET {
        $$ = new std::vector<ExpressionNode *>();
        $$->push_back($2);
    }
    |
    ArrRefs L_BRACKET Expression R_BRACKET {
        $1->push_back($3);
        $$ = $1;
    }
;

Condition:
    IF Expression THEN
    CompoundStatement
    ElseOrNot
    END IF {
        $$ = new IfNode(@1.first_line, @1.first_column, $2, $4, $5);
    }
;

ElseOrNot:
    ELSE
    CompoundStatement {
        $$ = $2;
    }
    |
    Epsilon {
        $$ = nullptr;
    }
;

While:
    WHILE Expression DO
    CompoundStatement
    END DO {
        $$ = new WhileNode(@1.first_line, @1.first_column, $2, $4);
    }
;

For:
    FOR ID ASSIGN INT_LITERAL TO INT_LITERAL DO
    CompoundStatement
    END DO {
        $$ = new ForNode(@1.first_line,
                         @1.first_column,
                         new DeclNode(@2.first_line, @2.first_column,
                                      new std::vector<VariableNode *>(1, new VariableNode(@2.first_line, @2.first_column, $2)),
                                      new Ptype(Ptype::PInt)),
                         new AssignmentNode(@3.first_line, @3.first_column,
                                            new VariableReferenceNode(@2.first_line, @2.first_column, $2),
                                            static_cast <ExpressionNode* > (new ConstantValueNode(@4.first_line, @4.first_column, $4))),
                         new ConstantValueNode(@6.first_line, @6.first_column, $6),
                         $8);
    }
;

Return:
    RETURN Expression SEMICOLON {
        $$ = new ReturnNode(@1.first_line, @1.first_column, $2);
    }
;

FunctionCall:
    FunctionInvocation SEMICOLON
;

FunctionInvocation:
    ID L_PARENTHESIS ExpressionList R_PARENTHESIS {
        $$ = new FunctionInvocationNode(@1.first_line, @1.first_column, $3, $1);
    }
;

ExpressionList:
    Epsilon {
        $$ = nullptr;
    }
    |
    Expressions {
        $$ = $1;
    }
;

Expressions:
    Expression {
        $$ = new std::vector<ExpressionNode *>();
        $$->push_back($1);
    }
    |
    Expressions COMMA Expression {
        $1->push_back($3);
        $$ = $1;
    }
;

StatementList:
    Epsilon {
        $$ = nullptr;
    }
    |
    Statements {
        $$ = $1;
    }
;

Statements:
    Statement {
        $$ = new std::vector<AstNode *>();
        $$->push_back($1);
    }
    |
    Statements Statement {
        $1->push_back($2);
        $$ = $1;
    }
;

Expression:
    L_PARENTHESIS Expression R_PARENTHESIS {
        $$ = $2;
    }
    |
    MINUS Expression %prec UNARY_MINUS {
        $$ = static_cast<ExpressionNode *>(new UnaryOperatorNode(@1.first_line, @1.first_column, "neg", $2));
    }
    |
    Expression MULTIPLY Expression {
        $$ = static_cast<ExpressionNode *>(new BinaryOperatorNode(@2.first_line, @2.first_column, "*", $1, $3));
    }
    |
    Expression DIVIDE Expression {
        $$ = static_cast<ExpressionNode *>(new BinaryOperatorNode(@2.first_line, @2.first_column, "/", $1, $3));
    }
    |
    Expression MOD Expression {
        $$ = static_cast<ExpressionNode *>(new BinaryOperatorNode(@2.first_line, @2.first_column, "mod", $1, $3));
    }
    |
    Expression PLUS Expression {
        $$ = static_cast<ExpressionNode *>(new BinaryOperatorNode(@2.first_line, @2.first_column, "+", $1, $3));
    }
    |
    Expression MINUS Expression {
        $$ = static_cast<ExpressionNode *>(new BinaryOperatorNode(@2.first_line, @2.first_column, "-", $1, $3));
    }
    |
    Expression LESS Expression {
        $$ = static_cast<ExpressionNode *>(new BinaryOperatorNode(@2.first_line, @2.first_column, "<", $1, $3));
    }
    |
    Expression LESS_OR_EQUAL Expression {
        $$ = static_cast<ExpressionNode *>(new BinaryOperatorNode(@2.first_line, @2.first_column, "<=", $1, $3));
    }
    |
    Expression GREATER Expression {
        $$ = static_cast<ExpressionNode *>(new BinaryOperatorNode(@2.first_line, @2.first_column, ">", $1, $3));
    }
    |
    Expression GREATER_OR_EQUAL Expression {
        $$ = static_cast<ExpressionNode *>(new BinaryOperatorNode(@2.first_line, @2.first_column, ">=", $1, $3));
    }
    |
    Expression EQUAL Expression {
        $$ = static_cast<ExpressionNode *>(new BinaryOperatorNode(@2.first_line, @2.first_column, "=", $1, $3));
    }
    |
    Expression NOT_EQUAL Expression {
        $$ = static_cast<ExpressionNode *>(new BinaryOperatorNode(@2.first_line, @2.first_column, "<>", $1, $3));
    }
    |
    NOT Expression {
        $$ = static_cast<ExpressionNode *>(new UnaryOperatorNode(@1.first_line, @1.first_column, "not", $2));
    }
    |
    Expression AND Expression {
        $$ = static_cast<ExpressionNode *>(new BinaryOperatorNode(@2.first_line, @2.first_column, "and", $1, $3));
    }
    |
    Expression OR Expression {
        $$ = static_cast<ExpressionNode *>(new BinaryOperatorNode(@2.first_line, @2.first_column, "or", $1, $3));
    }
    |
    IntegerAndReal {
        $$ = static_cast<ExpressionNode*>( $1 );
    }
    |
    StringAndBoolean {
        $$ = static_cast<ExpressionNode *>(new ConstantValueNode(@1.first_line, @1.first_column, $1));
    }
    |
    VariableReference {
        $$ = static_cast<ExpressionNode *>( $1 );
    }
    |
    FunctionInvocation {
        $$ = static_cast<ExpressionNode *>( $1 );
    }
;

    /*
       misc
            */
Epsilon:
;

%%

void yyerror(const char *msg) {
    fprintf(stderr,
            "\n"
            "|-----------------------------------------------------------------"
            "---------\n"
            "| Error found in Line #%d: %s\n"
            "|\n"
            "| Unmatched token: %s\n"
            "|-----------------------------------------------------------------"
            "---------\n",
            line_num, current_line, yytext);
    exit(-1);
}

int main(int argc, const char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename> [--dump-ast]\n", argv[0]);
        exit(-1);
    }

    yyin = fopen(argv[1], "r");
    if (yyin == NULL) {
        perror("fopen() failed");
        exit(-1);
    }

    yyparse();

    if (argc >= 3 && strcmp(argv[2], "--dump-ast") == 0) {
        AstDumper ast_dumper;
        root->accept(ast_dumper);
    }

    printf("\n"
           "|--------------------------------|\n"
           "|  There is no syntactic error!  |\n"
           "|--------------------------------|\n");

    delete root;
    fclose(yyin);
    yylex_destroy();
    return 0;
}
