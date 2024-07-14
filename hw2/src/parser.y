%{
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

extern int32_t line_num;    /* declared in scanner.l */
extern char current_line[]; /* declared in scanner.l */
extern FILE *yyin;          /* declared by lex */
extern char *yytext;        /* declared by lex */

extern int yylex(void);
static void yyerror(const char *msg);
extern int yylex_destroy(void);
%}
%token KWRETURN
%token KWEND KWBEGIN 
%token KWPRINT KWREAD KWIF KWELSE KWTHEN KWWHILE KWDO KWFOR KWTO KWOF KWDEF
%token KWVAR KWARRAY KWINTEGER KWBOOLEAN KWREAL KWSTRING
%token INTEGER KWTRUE KWFALSE OCT_INTEGER FLOAT SCIENTIFIC STRING
%token COLONEQUAL
%token LEFTBRACKET RIGHTBRACKET
%token SEMICOLON COLON COMMA
%token ID 

%token AND OR NOT
%token SMALLER SMALLEREQUAL SMALLERBIGGER BIGGEREQUAL BIGGER EQUAL
%left MINUS
%left PLUS
%left DIV MOD
%left MULT
%token LEFTPARENTHESE RIGHTPARENTHESE
%nonassoc UMINUS

%start program

%%
program: ID SEMICOLON
         variable_constant_declaration
         function_declaration_definition
         compound_statement
         KWEND ;

function_declaration: ID LEFTPARENTHESE formal_argument_list RIGHTPARENTHESE COLON scalar_type SEMICOLON 
                    | ID LEFTPARENTHESE formal_argument_list RIGHTPARENTHESE SEMICOLON 
                    ;
function_definition: ID LEFTPARENTHESE formal_argument_list RIGHTPARENTHESE COLON scalar_type
                     compound_statement
                     KWEND
                    |ID LEFTPARENTHESE formal_argument_list RIGHTPARENTHESE
                     compound_statement
                     KWEND
                    ;
function_declaration_definition: 
                               | function_declaration_definition_list
                               ;
function_declaration_definition_list: function_declaration
                                    | function_definition
                                    | function_declaration_definition_list function_declaration
                                    | function_declaration_definition_list function_definition
                                    ;
formal_argument: identifier_list COLON type 
               ;
formal_argument_list: 
                    | formal_arguments 
                    ;
formal_arguments: formal_argument
                | formal_arguments COMMA formal_argument
                ;
identifier_list: ID
               | identifier_list COMMA ID
               ;

variable_declaration: KWVAR identifier_list COLON scalar_type SEMICOLON
                    | KWVAR identifier_list COLON KWARRAY integer_constant KWOF type SEMICOLON
                    ;
integer_constant: INTEGER | OCT_INTEGER
                ;
constant_declaration: KWVAR identifier_list COLON literal_constant SEMICOLON
                    ;
literal_constant: INTEGER | KWTRUE | KWFALSE | OCT_INTEGER | FLOAT | SCIENTIFIC | STRING
                ;
variable_constant_declaration: 
                             | variable_constant_declaration_list
                             ;
variable_constant_declaration_list: variable_declaration
                                  | constant_declaration
                                  | variable_constant_declaration_list variable_declaration
                                  | variable_constant_declaration_list constant_declaration
                                  ;

statemet: compound_statement | simple_statement | conditional_statement | while_statement | for_statement | return_statemet | fucntion_call_statement
        ;
statemets: 
         | statemet_list
         ;
statemet_list: statemet
             | statemet_list statemet
             ;  
compound_statement: KWBEGIN
                    variable_constant_declaration
                    statemets
                    KWEND
                    ;
simple_statement: variable_reference COLONEQUAL expression SEMICOLON
                | KWPRINT expression SEMICOLON
                | KWREAD variable_reference SEMICOLON
                ;
variable_reference: ID
                  | ID array_indexs
                  ;
array_index_number: expression
                  ;
array_indexs: LEFTBRACKET array_index_number RIGHTBRACKET
            | array_indexs LEFTBRACKET integer_constant RIGHTBRACKET
            ;
conditional_statement: KWIF expression KWTHEN
                       compound_statement
                       KWELSE
                       compound_statement
                       KWEND KWIF
                     | KWIF expression KWTHEN
                       compound_statement
                       KWEND KWIF
                     ;
while_statement: KWWHILE expression KWDO
                 compound_statement
                 KWEND KWDO
                 ;
for_statement: KWFOR ID COLONEQUAL integer_constant KWTO integer_constant KWDO
               compound_statement
               KWEND KWDO
return_statemet: KWRETURN expression SEMICOLON
               ;
fucntion_call_statement: ID LEFTPARENTHESE expressions RIGHTPARENTHESE SEMICOLON
                        ;
function_call_nosemi: ID LEFTPARENTHESE expressions RIGHTPARENTHESE
                    ;
expressions: 
           | expression_list
           ;
expression_list: expression
               | expression_list COMMA expression
               ;
expression: literal_constant | variable_reference | function_call_nosemi | arithmetic_expression;
arithmetic_expression: MINUS expression %prec UMINUS
                     | expression MULT expression
                     | expression DIV expression
                     | expression MOD expression
                     | expression PLUS expression
                     | expression MINUS expression
                     | expression SMALLER expression
                     | expression SMALLEREQUAL expression
                     | expression SMALLERBIGGER expression
                     | expression BIGGEREQUAL expression
                     | expression BIGGER expression
                     | expression EQUAL expression
                     | expression AND expression
                     | expression OR expression
                     | NOT expression
                     | LEFTPARENTHESE expression RIGHTPARENTHESE
                     ;
scalar_type:  KWINTEGER
    | KWREAL
    | KWSTRING
    | KWBOOLEAN
    ;
type: scalar_type
    | KWARRAY integer_constant KWOF type
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
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(-1);
    }

    yyin = fopen(argv[1], "r");
    if (yyin == NULL) {
        perror("fopen() failed");
        exit(-1);
    }

    yyparse();

    fclose(yyin);
    yylex_destroy();

    printf("\n"
           "|--------------------------------|\n"
           "|  There is no syntactic error!  |\n"
           "|--------------------------------|\n");
    return 0;
}
