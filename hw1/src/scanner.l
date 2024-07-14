%{
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define LIST_SOURCE                 appendToCurrentLine(yytext)
#define LIST_TOKEN(name)            do { LIST_SOURCE; if(opt_tok) printf("<%s>\n", name); } while(0)
#define LIST_LITERAL(name, literal) do { LIST_SOURCE; if(opt_tok) printf("<%s: %s>\n", name, literal); } while(0)
#define MAX_LINE_LENG               512

uint32_t line_num = 1;
static char current_line[MAX_LINE_LENG];

static uint32_t opt_src = 1;
static uint32_t opt_tok = 1;
static char *current_line_ptr = current_line;

static void appendToCurrentLine(const char *yytext_ptr);
%}

id ([a-zA-Z][a-zA-Z0-9]*)
integer ([1-9][0-9]*|[0])
oct_integer ([0][0-8]*)
float ({integer}[.][0-9]*[1-9]|{integer}[.][0])
scientific ([1-9][0-9]*|[1-9][0-9]*[.][0-9]*[1-9]|[1-9][0-9]*[.][0])[Ee][+-]?{integer}
string ["](([^"\n]|["]["])*)["]
comment_c [/][*]([^*]|[*]+[^/])*[*][/]
comment_cpp [/][/]([^\n])*
spp [/][/][&][S][+][^\n]*
smm [/][/][&][S][-][^\n]*
tpp [/][/][&][T][+][^\n]*
tmm [/][/][&][T][-][^\n]*

%%
{spp} {
    opt_src = 1;
    LIST_SOURCE;
}

{smm} {
    opt_src = 0;
    LIST_SOURCE;
}

{tpp} {
    opt_tok = 1;
    LIST_SOURCE;
}

{tmm} {
    opt_tok = 0;
    LIST_SOURCE;
}

{comment_c} {
    while(*yytext) {
        *current_line_ptr = *yytext;
        ++current_line_ptr;
        ++yytext;
        if (*(current_line_ptr-1) == '\n') {
            *current_line_ptr = '\0';
            if (opt_src) {
                printf("%d: %s", line_num, current_line);
            }
            ++line_num;
            for (int i=0; i<MAX_LINE_LENG; i++) {
                current_line[i] = '\0';
            }
            current_line_ptr = current_line;
        }
    }
}

{comment_cpp} {
    LIST_SOURCE;
}

"," { LIST_TOKEN(","); }
":" { LIST_TOKEN(":"); }
";" { LIST_TOKEN(";"); }
"(" { LIST_TOKEN("("); }
")" { LIST_TOKEN(")"); }
"[" { LIST_TOKEN("["); }
"]" { LIST_TOKEN("]"); }

"+" { LIST_TOKEN("+"); }
"-" { LIST_TOKEN("-"); }
"*" { LIST_TOKEN("*"); }
"/" { LIST_TOKEN("/"); }
"mod" { LIST_TOKEN("mod"); }
":=" { LIST_TOKEN(":="); }
"<" { LIST_TOKEN("<"); }
"<=" { LIST_TOKEN("<="); }
"<>" { LIST_TOKEN("<>"); }
">=" { LIST_TOKEN(">="); }
">" { LIST_TOKEN(">"); }
"=" { LIST_TOKEN("="); }
"and" { LIST_TOKEN("and"); }
"or" { LIST_TOKEN("or"); }
"not" { LIST_TOKEN("not"); }

"var" { LIST_TOKEN("KWvar"); }
"def" { LIST_TOKEN("KWdef"); }
"array" { LIST_TOKEN("KWarray"); }
"of" { LIST_TOKEN("KWof"); }
"boolean" { LIST_TOKEN("KWboolean"); }
"integer" { LIST_TOKEN("KWinteger"); }
"real" { LIST_TOKEN("KWreal"); }
"string" { LIST_TOKEN("KWstring"); }
"true" { LIST_TOKEN("KWtrue"); }
"false" { LIST_TOKEN("KWfalse"); }
"while" { LIST_TOKEN("KWwhile"); }
"do" { LIST_TOKEN("KWdo"); }
"if" { LIST_TOKEN("KWif"); }
"then" { LIST_TOKEN("KWthen"); }
"else" { LIST_TOKEN("KWelse"); }
"for" { LIST_TOKEN("KWfor"); }
"to" { LIST_TOKEN("KWto"); }
"begin" { LIST_TOKEN("KWbegin"); }
"end" { LIST_TOKEN("KWend"); }
"print" { LIST_TOKEN("KWprint"); }
"read" { LIST_TOKEN("KWread"); }
"return" { LIST_TOKEN("KWreturn"); }

{id} {
    LIST_LITERAL("id", yytext);
}

{integer} {
    LIST_LITERAL("integer", yytext);
}

{oct_integer} {
    LIST_LITERAL("oct_integer", yytext);
}

{float} {
    LIST_LITERAL("float", yytext);
}

{scientific} {
    LIST_LITERAL("scientific", yytext);
}

{string} {
    char str[MAX_LINE_LENG];
    int idx = 0;
    for (int i=0; i<yyleng; i++) {
        if (i == 0 && yytext[i] == '"') continue;
        else if (i == yyleng-1 && yytext[i] == '"') continue;
        else if (yytext[i] == '"' && yytext[i+1] == '"') {
            str[idx] = yytext[i];
            ++idx;
            ++i;
        }
        else {
            str[idx] = yytext[i];
            ++idx;
        }
    }
    str[idx] = '\0';
    LIST_LITERAL("string", str);
}

\n {
    if (opt_src) {
        printf("%d: %s\n", line_num, current_line);
    }
    ++line_num;
    current_line[0] = '\0';
    current_line_ptr = current_line;
}

[ \t] {
    LIST_SOURCE;
}
    /* Catch the character which is not accepted by rules above */
. {
    printf("Error at line %d: bad character \"%s\"\n", line_num, yytext);
    exit(-1);
}

%%

static void appendToCurrentLine(const char *yytext_ptr) {
    while (*yytext_ptr) {
        *current_line_ptr = *yytext_ptr;
        ++current_line_ptr;
        ++yytext_ptr;
    }
    *current_line_ptr = '\0';
}

int main(int argc, const char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s [filename]\n", argv[0]);
        exit(-1);
    }

    yyin = fopen(argv[1], "r");
    if (yyin == NULL) {
        fprintf(stderr, "Open file error\n");
        exit(-1);
    }

    yylex();

    fclose(yyin);
    /* Free resources used by the scanner; otherwise, unused data structures
    will persist throughout the successive compiler phases. */
    yylex_destroy();

    return 0;
}
