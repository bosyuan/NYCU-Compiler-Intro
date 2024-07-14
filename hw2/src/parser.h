/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_PARSER_H_INCLUDED
# define YY_YY_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    KWRETURN = 258,
    KWEND = 259,
    KWBEGIN = 260,
    KWPRINT = 261,
    KWREAD = 262,
    KWIF = 263,
    KWELSE = 264,
    KWTHEN = 265,
    KWWHILE = 266,
    KWDO = 267,
    KWFOR = 268,
    KWTO = 269,
    KWOF = 270,
    KWDEF = 271,
    KWVAR = 272,
    KWARRAY = 273,
    KWINTEGER = 274,
    KWBOOLEAN = 275,
    KWREAL = 276,
    KWSTRING = 277,
    INTEGER = 278,
    KWTRUE = 279,
    KWFALSE = 280,
    OCT_INTEGER = 281,
    FLOAT = 282,
    SCIENTIFIC = 283,
    STRING = 284,
    COLONEQUAL = 285,
    LEFTBRACKET = 286,
    RIGHTBRACKET = 287,
    SEMICOLON = 288,
    COLON = 289,
    COMMA = 290,
    ID = 291,
    AND = 292,
    OR = 293,
    NOT = 294,
    SMALLER = 295,
    SMALLEREQUAL = 296,
    SMALLERBIGGER = 297,
    BIGGEREQUAL = 298,
    BIGGER = 299,
    EQUAL = 300,
    MINUS = 301,
    PLUS = 302,
    DIV = 303,
    MOD = 304,
    MULT = 305,
    LEFTPARENTHESE = 306,
    RIGHTPARENTHESE = 307,
    UMINUS = 308
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_H_INCLUDED  */
