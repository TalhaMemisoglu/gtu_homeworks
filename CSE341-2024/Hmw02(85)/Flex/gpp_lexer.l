%{
#include <stdio.h>
%}

/* Define Tokens */
%%
"and"                       { printf("KW_AND\n"); }
"or"                        { printf("KW_OR\n"); }
"not"                       { printf("KW_NOT\n"); }
"equal"                     { printf("KW_EQUAL\n"); }
"less"                      { printf("KW_LESS\n"); }
"nil"                       { printf("KW_NIL\n"); }
"list"                      { printf("KW_LIST\n"); }
"append"                    { printf("KW_APPEND\n"); }
"concat"                    { printf("KW_CONCAT\n"); }
"set"                       { printf("KW_SET\n"); }
"deffun"                    { printf("KW_DEFFUN\n"); }
"for"                       { printf("KW_FOR\n"); }
"if"                        { printf("KW_IF\n"); }
"exit"                      { printf("KW_EXIT\n"); }
"load"                      { printf("KW_LOAD\n"); }
"print"                     { printf("KW_DISP\n"); }
"true"                      { printf("KW_TRUE\n"); }
"false"                     { printf("KW_FALSE\n"); }
"+"                         { printf("OP_PLUS\n"); }
"-"                         { printf("OP_MINUS\n"); }
"/"                         { printf("OP_DIV\n"); }
"*"                         { printf("OP_MULT\n"); }
"("                         { printf("OP_OP\n"); }
")"                         { printf("OP_CP\n"); }
","                         { printf("OP_COMMA\n"); }
";;".*                      { printf("COMMENT\n"); }  /* Matches comments starting with ';;' */
[0-9]+                      { printf("VALUEI\n"); }  /* Unsigned integers */
[0-9]+":"[0-9]+             { printf("VALUEF\n"); } /* Unsigned fractions (e.g., 123:12) */
[a-zA-Z_][a-zA-Z0-9_]*      { printf("IDENTIFIER\n"); }  /* Identifiers */
[ \t\n]+                    ;   /* Ignore whitespace */
.                           { printf("SYNTAX_ERROR %s cannot be tokenized!\n", yytext); }
%%

/* Main function to initialize the lexer */
int main(void) {
    printf("Enter G++ expressions (Ctrl+D to end):\n");
    yylex();  // Starts reading and tokenizing input
    return 0;
}
