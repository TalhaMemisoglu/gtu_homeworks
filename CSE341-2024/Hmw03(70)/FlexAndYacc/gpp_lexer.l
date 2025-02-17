%{
    #include<stdio.h> 
    #include "gpp_interpreter.tab.h"
    #include <string.h>
%}

/* Define Tokens */
%%
"and"                        return KW_AND; 
"or"                         return KW_OR; 
"not"                        return KW_NOT; 
"equal"                      return KW_EQUAL; 
"less"                       return KW_LESS; 
"nil"                        return KW_NIL; 
"list"                       return KW_LIST; 
"append"                     return KW_APPEND; 
"concat"                     return KW_CONCAT; 
"set"                        return KW_SET; 
"deffun"                     return KW_DEFFUN;
"defvar"                     return KW_DEFVAR; 
"for"                        return KW_FOR;
"while"                      return KW_WHILE; 
"if"                         return KW_IF; 
"exit"                       return KW_EXIT; 
"load"                       return KW_LOAD; 
"print"                      return KW_DISP; 
"true"                       return KW_TRUE; 
"false"                      return KW_FALSE; 
"+"                          return OP_PLUS; 
"-"                          return OP_MINUS; 
"/"                          return OP_DIV; 
"*"                          return OP_MULT;
"^"		                     return OP_POW; 
"("                          return OP_OP; 
")"                          return OP_CP; 
","                          return OP_COMMA;
"^"                          return OP_POW; 
"'("                         return OP_LIST;
";;".*                       return 0;   /* Matches comments starting with ';;' */
[0-9]+                       {yylval.value=atoi(yytext); return VALUEI;}   /* Unsigned integers */
[0-9]+":"[0-9]+              {yylval.value=atoi(yytext); return VALUEF;}  /* Unsigned fractions (e.g., 123:12) */
[a-zA-Z_][a-zA-Z0-9_]*       {strcpy(yylval.id,yytext); return IDENTIFIER;}   /* Identifiers */
[ \t\n]+                    {};   /* Ignore whitespace */
.                            printf("SYNTAX_ERROR %s cannot be tokenized!\n", yytext); 
%%

int yywrap(){ 
    return 1; 
}
