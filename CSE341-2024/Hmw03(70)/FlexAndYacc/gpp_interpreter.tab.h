/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_GPP_INTERPRETER_TAB_H_INCLUDED
# define YY_YY_GPP_INTERPRETER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    KW_AND = 258,                  /* KW_AND  */
    KW_OR = 259,                   /* KW_OR  */
    KW_NOT = 260,                  /* KW_NOT  */
    KW_EQUAL = 261,                /* KW_EQUAL  */
    KW_LESS = 262,                 /* KW_LESS  */
    KW_NIL = 263,                  /* KW_NIL  */
    KW_LIST = 264,                 /* KW_LIST  */
    KW_APPEND = 265,               /* KW_APPEND  */
    KW_CONCAT = 266,               /* KW_CONCAT  */
    KW_SET = 267,                  /* KW_SET  */
    KW_DEFFUN = 268,               /* KW_DEFFUN  */
    KW_DEFVAR = 269,               /* KW_DEFVAR  */
    KW_FOR = 270,                  /* KW_FOR  */
    KW_WHILE = 271,                /* KW_WHILE  */
    KW_IF = 272,                   /* KW_IF  */
    KW_EXIT = 273,                 /* KW_EXIT  */
    KW_LOAD = 274,                 /* KW_LOAD  */
    KW_DISP = 275,                 /* KW_DISP  */
    KW_TRUE = 276,                 /* KW_TRUE  */
    KW_FALSE = 277,                /* KW_FALSE  */
    OP_PLUS = 278,                 /* OP_PLUS  */
    OP_MINUS = 279,                /* OP_MINUS  */
    OP_DIV = 280,                  /* OP_DIV  */
    OP_MULT = 281,                 /* OP_MULT  */
    OP_POW = 282,                  /* OP_POW  */
    OP_OP = 283,                   /* OP_OP  */
    OP_CP = 284,                   /* OP_CP  */
    OP_COMMA = 285,                /* OP_COMMA  */
    OP_DBLQUOTE = 286,             /* OP_DBLQUOTE  */
    OP_LIST = 287,                 /* OP_LIST  */
    COMMENT = 288,                 /* COMMENT  */
    VALUEI = 289,                  /* VALUEI  */
    VALUEF = 290,                  /* VALUEF  */
    IDENTIFIER = 291               /* IDENTIFIER  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 145 ".\\gpp_interpreter.y"

    int value;
    void* values;
    char id[50];

#line 106 "gpp_interpreter.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_GPP_INTERPRETER_TAB_H_INCLUDED  */
