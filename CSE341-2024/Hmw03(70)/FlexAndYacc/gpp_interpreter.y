%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
void yyERROR(const char *s);
int yylex(void);

#define YYDEBUG 1
extern int yydebug;


extern FILE *yyin;

int EXITPROG = 0;
int ERROR = 0;
int NUMFLAG = 0;
int PRINTFLAG = 0;
int LISTFLAG = 0;

int arr[1000]; 
int idx=0;
int indexArr=0;

typedef struct{
    int val;  //key part
    char id[30];  // value part
}pair;

pair symbolTable[100];
int indexST=0;

void appendToSymbolTable(int value, char iden[30]){
    pair x;
    x.val = value;
    strcpy(x.id,iden);
    for(int i=0;i<indexST;i++){
        if(strcmp(x.id,symbolTable[i].id)==0){
            symbolTable[i].val = x.val;
            return;
        }
    }
    symbolTable[indexST++] = x;
}

int getValue(char iden[30]){
    for(int i=0;i<indexST;i++){
        if(strcmp(iden,symbolTable[i].id)==0){
            return symbolTable[i].val;
        }
    }
    printf("ERROR: %s is not defined.",iden);
    ERROR=1;
}

void printArr(int arr[],int length){
    int i;
    for(i=0;i<length;i++){
        if(i!= length -1)
            printf("%d,",arr[i]);
        else    
             printf("%d",arr[i]);    

    }

}
int* append(int arr[],int length,int val){
    int i;
    for (i = length-1; i>=0; i--){
        arr[i+1]=arr[i];


    }
    arr[0]=val;
    return arr;

}

void printEXPI(int num){
    if(!ERROR){
        printf("\nSYNTAX OK") ;
        if(PRINTFLAG==1){
            if(LISTFLAG==1){ 
                printf ("Result: (" );
                printArr(arr,indexArr);
                printf (")\n");
                indexArr=0;
                LISTFLAG=0;
            }
            else if(NUMFLAG==1){
                printf("\nResult:%d\n", num);
                NUMFLAG=0;
            }
            else{ 
                printf("Result: Nil\n"); 
                LISTFLAG=0;
                NUMFLAG=0;
            }
            PRINTFLAG=0;
        }
    } 
}

void printEXPLISTI(){
    if(PRINTFLAG==1 &&LISTFLAG==1 &&!ERROR){ 
        printf("\nSYNTAX OK.\n");
        printf ("Result: (" );
        printArr(arr,indexArr);
        printf (")\n");
        indexArr=0;
        PRINTFLAG=0;
        LISTFLAG=0;
        idx=0;
    }
}

void printEXPB(int num){
    if (!ERROR)
        printf("\nSYNTAX OK."); 
    if(PRINTFLAG==1 &&!ERROR){
        printf("\nResult: ");
        if(num == 1){
            printf ("true\n");
        }
        else
            printf("false\n");  
        PRINTFLAG=0; 
        LISTFLAG=0;
        NUMFLAG=0; 
        idx=0;
    }     
}

%}

/* Declare tokens from your lexer */
%token KW_AND KW_OR KW_NOT KW_EQUAL KW_LESS KW_NIL
%token KW_LIST KW_APPEND KW_CONCAT KW_SET KW_DEFFUN KW_DEFVAR
%token KW_FOR KW_WHILE KW_IF KW_EXIT KW_LOAD KW_DISP
%token KW_TRUE KW_FALSE
%token OP_PLUS OP_MINUS OP_DIV OP_MULT OP_POW
%token OP_OP OP_CP OP_COMMA OP_DBLQUOTE OP_LIST
%token COMMENT

%union{
    int value;
    void* values;
    char id[50];
}

%token <value> VALUEI
%token <value> VALUEF
%type <value> NUMBER
%token <id> IDENTIFIER
%type <value> INPUT
%type <value> EXPI
%type <value> EXPB
%type <value> EXP
%type <value> BINARYVALUE
%type <values> EXPLISTI
%type <values> LISTVALUE
%type <values> VALUES


/* Define operator precedence and associativity */
%left KW_OR
%left KW_AND
%left KW_NOT
%left KW_EQUAL KW_LESS
%left OP_PLUS OP_MINUS
%left OP_MULT OP_DIV

%%

START       :      
            | INPUT
            ;

INPUT       : EXPI      { printEXPI($$); }
            | EXPLISTI  { printEXPLISTI(); }
            | EXPB      { printEXPB($$); }
            ;

LISTVALUE   : OP_LIST VALUES OP_CP          { if (idx==0 && !ERROR) { idx=indexArr; }}
            | OP_LIST OP_CP                 {$$=0;}
            | OP_OP KW_LIST VALUES OP_CP    {PRINTFLAG=1; LISTFLAG=1; $$=$3;}
            ;

VALUES      : VALUES NUMBER { arr[indexArr]=$2; indexArr=indexArr+1; }
            | NUMBER        { arr[indexArr]=$1 ; indexArr=indexArr+1; }
            ;

NUMBER      : VALUEI
            | VALUEF 
            ;

EXPI        : OP_OP OP_PLUS EXPI EXPI OP_CP {$$=$3+$4; PRINTFLAG=1; NUMFLAG=1;}
            | OP_OP OP_MINUS EXPI EXPI OP_CP {$$=$3-$4; PRINTFLAG=1; NUMFLAG=1;}
            | OP_OP OP_MULT EXPI EXPI OP_CP {$$=$3*$4; PRINTFLAG=1; NUMFLAG=1;}
            | OP_OP OP_DIV EXPI EXPI OP_CP {if($4 != 0){$$=$3/$4;} PRINTFLAG=1; NUMFLAG=1;}
            | OP_OP OP_POW EXPI EXPI OP_CP { pow($3,$4); PRINTFLAG=1; NUMFLAG=1;}
            | OP_OP IDENTIFIER EXP OP_CP { $$= $3; LISTFLAG=1; PRINTFLAG=1;  }
            | OP_OP KW_SET IDENTIFIER EXPI OP_CP { $$ = $4; appendToSymbolTable($4,$3); NUMFLAG=1; PRINTFLAG=1;}
            | OP_OP KW_DEFFUN IDENTIFIER IDENTIFIERLIST EXP OP_CP {$$ = 0;}
            | IDENTIFIER {$$ = getValue($1); NUMFLAG=1; PRINTFLAG=1;}
            | OP_OP KW_LOAD STRING OP_CP {$$=0;}
            | OP_OP KW_DISP EXPI OP_CP {$$ = $3; NUMFLAG=1; PRINTFLAG=1;}
            | NUMBER {$$=$1;}
            | STRING {$$ = 0;}
            | OP_OP KW_IF EXPB EXP OP_CP  {
                if($3==1)
                    $$ = $4;
            }
            | OP_OP KW_IF EXPB EXP EXP OP_CP {
                if($3==1)
                    $$ = $4;
                else
                    $$ = $5;
            }
            | OP_OP KW_WHILE EXPB EXP OP_CP {$$ = 0;}
            | OP_OP KW_FOR OP_OP IDENTIFIER EXPI EXPI OP_CP EXP OP_CP {$$ = 0;}
            | OP_OP KW_DEFVAR IDENTIFIER EXPI OP_CP {  PRINTFLAG=1; NUMFLAG=1; $$=$4; }
            | OP_OP KW_EXIT OP_CP { EXITPROG=1; return 0;}
            ;

EXPB        : OP_OP KW_AND EXPB EXPB OP_CP {$$=($3 && $4);  PRINTFLAG=1;} 
            | OP_OP KW_OR EXPB EXPB OP_CP {$$=($3 || $4); PRINTFLAG=1; }
            | OP_OP KW_NOT EXPB OP_CP {$$=!$3;  PRINTFLAG=1;}
            | OP_OP KW_LESS EXPI EXPI OP_CP { $$=($3 < $4); PRINTFLAG=1; }
            | OP_OP KW_EQUAL EXPB EXPB OP_CP {$$=($3==$4);  PRINTFLAG=1; }
            | OP_OP KW_EQUAL EXPI EXPI OP_CP {$$=($3==$4);  PRINTFLAG=1; }
            | BINARYVALUE
            ;

BINARYVALUE : KW_TRUE  { $$=1;}
            | KW_FALSE {$$=0; }
            ;

IDLIST      : IDENTIFIER 
            | IDENTIFIER IDLIST
            ;

IDENTIFIERLIST : OP_OP IDLIST OP_CP
            ;

EXPLISTI    : OP_OP KW_CONCAT EXPLISTI EXPLISTI OP_CP {$$=0;}
            | OP_OP KW_APPEND EXPI EXPLISTI OP_CP { append(arr,indexArr,$3); indexArr=indexArr+1; }
            | LISTVALUE { LISTFLAG=1; PRINTFLAG=1;}
            ;

EXP         : EXPLISTI  {$$ = $1;}
            | EXPI      {$$ = $1;}
            | EXPB      {$$ = $1;}
            ;

STRING      : OP_DBLQUOTE IDENTIFIER OP_DBLQUOTE
            ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "ERROR: %s\n", s);
}

int main(void) {
    

    FILE *fp; 
    char userInput[50];
    char file[50];

    printf("Write g++ in order to start the interpreter.\n");
    printf("Write g++ <filename> in order to interprete a file.\n");
    printf("$ "); 
    
    fgets(userInput,sizeof userInput,stdin); //user input from terminal
    if (strncmp(userInput,"g++",3) !=0 ){//checking if first 3 letters are g++
        printf("Wrong userInput!");    
    }else{
        if(strlen(userInput) == 4){
            printf("Interpreter Mode:\n");   
            yyin = stdin; //Input from terminal
            while (EXITPROG == 0){
                printf(">");
                yyparse();
            }
        }else{
            printf("File Mode:\n");
            strncpy(file, userInput+4,strlen(userInput)-4);

            file[strlen(userInput)-5] = '\0';
            printf ("$File name: %s\n",file);
            fp = fopen(file,"r"); 
            if(fp == NULL){
                printf("ERROR: No such file exists.");
                return 0;
            }
            yyin = fp; //Input from file
            while (EXITPROG == 0 || !feof(yyin)){
                yyparse();
            }   
        }
    }

    return 0;
}