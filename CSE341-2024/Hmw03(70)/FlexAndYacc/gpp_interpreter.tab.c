/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 ".\\gpp_interpreter.y"

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


#line 206 "gpp_interpreter.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "gpp_interpreter.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_KW_AND = 3,                     /* KW_AND  */
  YYSYMBOL_KW_OR = 4,                      /* KW_OR  */
  YYSYMBOL_KW_NOT = 5,                     /* KW_NOT  */
  YYSYMBOL_KW_EQUAL = 6,                   /* KW_EQUAL  */
  YYSYMBOL_KW_LESS = 7,                    /* KW_LESS  */
  YYSYMBOL_KW_NIL = 8,                     /* KW_NIL  */
  YYSYMBOL_KW_LIST = 9,                    /* KW_LIST  */
  YYSYMBOL_KW_APPEND = 10,                 /* KW_APPEND  */
  YYSYMBOL_KW_CONCAT = 11,                 /* KW_CONCAT  */
  YYSYMBOL_KW_SET = 12,                    /* KW_SET  */
  YYSYMBOL_KW_DEFFUN = 13,                 /* KW_DEFFUN  */
  YYSYMBOL_KW_DEFVAR = 14,                 /* KW_DEFVAR  */
  YYSYMBOL_KW_FOR = 15,                    /* KW_FOR  */
  YYSYMBOL_KW_WHILE = 16,                  /* KW_WHILE  */
  YYSYMBOL_KW_IF = 17,                     /* KW_IF  */
  YYSYMBOL_KW_EXIT = 18,                   /* KW_EXIT  */
  YYSYMBOL_KW_LOAD = 19,                   /* KW_LOAD  */
  YYSYMBOL_KW_DISP = 20,                   /* KW_DISP  */
  YYSYMBOL_KW_TRUE = 21,                   /* KW_TRUE  */
  YYSYMBOL_KW_FALSE = 22,                  /* KW_FALSE  */
  YYSYMBOL_OP_PLUS = 23,                   /* OP_PLUS  */
  YYSYMBOL_OP_MINUS = 24,                  /* OP_MINUS  */
  YYSYMBOL_OP_DIV = 25,                    /* OP_DIV  */
  YYSYMBOL_OP_MULT = 26,                   /* OP_MULT  */
  YYSYMBOL_OP_POW = 27,                    /* OP_POW  */
  YYSYMBOL_OP_OP = 28,                     /* OP_OP  */
  YYSYMBOL_OP_CP = 29,                     /* OP_CP  */
  YYSYMBOL_OP_COMMA = 30,                  /* OP_COMMA  */
  YYSYMBOL_OP_DBLQUOTE = 31,               /* OP_DBLQUOTE  */
  YYSYMBOL_OP_LIST = 32,                   /* OP_LIST  */
  YYSYMBOL_COMMENT = 33,                   /* COMMENT  */
  YYSYMBOL_VALUEI = 34,                    /* VALUEI  */
  YYSYMBOL_VALUEF = 35,                    /* VALUEF  */
  YYSYMBOL_IDENTIFIER = 36,                /* IDENTIFIER  */
  YYSYMBOL_YYACCEPT = 37,                  /* $accept  */
  YYSYMBOL_START = 38,                     /* START  */
  YYSYMBOL_INPUT = 39,                     /* INPUT  */
  YYSYMBOL_LISTVALUE = 40,                 /* LISTVALUE  */
  YYSYMBOL_VALUES = 41,                    /* VALUES  */
  YYSYMBOL_NUMBER = 42,                    /* NUMBER  */
  YYSYMBOL_EXPI = 43,                      /* EXPI  */
  YYSYMBOL_EXPB = 44,                      /* EXPB  */
  YYSYMBOL_BINARYVALUE = 45,               /* BINARYVALUE  */
  YYSYMBOL_IDLIST = 46,                    /* IDLIST  */
  YYSYMBOL_IDENTIFIERLIST = 47,            /* IDENTIFIERLIST  */
  YYSYMBOL_EXPLISTI = 48,                  /* EXPLISTI  */
  YYSYMBOL_EXP = 49,                       /* EXP  */
  YYSYMBOL_STRING = 50                     /* STRING  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  45
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   201

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  37
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  14
/* YYNRULES -- Number of rules.  */
#define YYNRULES  51
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  133

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   291


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   175,   175,   176,   179,   180,   181,   184,   185,   186,
     189,   190,   193,   194,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   214,   220,
     221,   222,   223,   226,   227,   228,   229,   230,   231,   232,
     235,   236,   239,   240,   243,   246,   247,   248,   251,   252,
     253,   256
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "KW_AND", "KW_OR",
  "KW_NOT", "KW_EQUAL", "KW_LESS", "KW_NIL", "KW_LIST", "KW_APPEND",
  "KW_CONCAT", "KW_SET", "KW_DEFFUN", "KW_DEFVAR", "KW_FOR", "KW_WHILE",
  "KW_IF", "KW_EXIT", "KW_LOAD", "KW_DISP", "KW_TRUE", "KW_FALSE",
  "OP_PLUS", "OP_MINUS", "OP_DIV", "OP_MULT", "OP_POW", "OP_OP", "OP_CP",
  "OP_COMMA", "OP_DBLQUOTE", "OP_LIST", "COMMENT", "VALUEI", "VALUEF",
  "IDENTIFIER", "$accept", "START", "INPUT", "LISTVALUE", "VALUES",
  "NUMBER", "EXPI", "EXPB", "BINARYVALUE", "IDLIST", "IDENTIFIERLIST",
  "EXPLISTI", "EXP", "STRING", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-52)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     165,   -52,   -52,   108,   -33,   -24,   -52,   -52,   -52,     6,
     -52,   -52,   -52,   -52,   -52,   -52,   -52,   -52,    -3,    -3,
      -3,    -5,   -27,    28,   -27,    25,   -21,   -16,    -8,     5,
      -3,    -3,    13,    19,   -27,   -27,   -27,   -27,   -27,   -27,
     165,    46,   -52,    12,   -52,   -52,    94,    -3,    -3,    16,
     133,   -27,    -3,   158,   -27,    14,    25,    56,    25,   -27,
      50,   -27,    20,   165,   165,   -52,    51,    54,   -27,   -27,
     -27,   -27,   -27,   -52,   -52,   -52,    58,   -52,   -52,   -52,
      61,    75,   -52,    76,    77,    79,   -52,    80,    81,   100,
      48,   165,   101,   -27,   112,    60,   -52,   -52,   113,   114,
     125,   126,   132,   -52,   -52,   -52,   -52,   -52,   -52,   -52,
     -52,   -52,    48,   134,   135,   -52,   -27,   -52,   -52,   136,
     -52,   -52,   -52,   -52,   -52,   -52,   -52,   -52,   137,   -52,
     165,   138,   -52
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,    40,    41,     0,     0,     0,    12,    13,    22,     0,
       3,    47,    25,     4,     6,    39,     5,    26,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     8,     0,    11,     1,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    32,     0,     0,     0,     0,
       0,     0,     0,    49,    50,    48,     0,    51,     7,    10,
       0,     0,    35,     0,     0,     0,     9,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    23,    24,     0,     0,
       0,     0,     0,    19,    33,    34,    38,    37,    36,    46,
      45,    20,    42,     0,     0,    31,     0,    29,    27,     0,
      14,    15,    17,    16,    18,    43,    44,    21,     0,    28,
       0,     0,    30
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -52,   -52,   -52,   -52,   139,     9,     0,    55,   -52,    67,
     -52,     2,   -51,   147
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     9,    10,    11,    43,    12,    73,    74,    15,   113,
      91,    75,    76,    17
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      13,    53,    16,    41,     4,    42,    45,     6,     7,     8,
       6,     7,    94,    95,    44,    59,     1,     2,     1,     2,
      60,    51,    54,    50,    56,    46,     4,    58,    61,     6,
       7,     8,    44,    62,    67,    68,    69,    70,    71,    72,
     114,    78,    65,    86,   119,    82,     6,     7,     6,     7,
       4,    83,    79,    57,    85,    14,    93,     5,    87,    89,
      88,    92,     6,     7,    79,    23,    24,    25,    98,    99,
     100,   101,   102,    47,    48,    49,    52,    77,    90,   131,
      96,     1,     2,    97,   112,    63,    64,   103,     3,   118,
     104,     4,     5,   116,     6,     7,     8,    18,    19,    20,
      21,    22,    80,    81,   105,   106,   107,    84,   108,   109,
     110,    18,    19,    20,    21,    22,   128,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,   111,
     115,    35,    36,    37,    38,    39,    18,    19,    20,    21,
      22,   117,   120,   121,    40,    26,    27,    28,    29,    30,
      31,    32,    33,    34,   122,   123,    35,    36,    37,    38,
      39,   124,    55,   126,   127,   129,   130,   132,     0,    40,
      26,    27,    28,    29,    30,    31,    32,    33,    34,   125,
      66,    35,    36,    37,    38,    39,     1,     2,     0,     0,
       0,     0,     0,     3,    40,     0,     4,     5,     0,     6,
       7,     8
};

static const yytype_int16 yycheck[] =
{
       0,    28,     0,    36,    31,    29,     0,    34,    35,    36,
      34,    35,    63,    64,     5,    36,    21,    22,    21,    22,
      36,    21,    22,    28,    24,    28,    31,    25,    36,    34,
      35,    36,    23,    28,    34,    35,    36,    37,    38,    39,
      91,    29,    29,    29,    95,    29,    34,    35,    34,    35,
      31,    51,    43,    28,    54,     0,    36,    32,    56,    59,
      58,    61,    34,    35,    55,     9,    10,    11,    68,    69,
      70,    71,    72,    18,    19,    20,    21,    31,    28,   130,
      29,    21,    22,    29,    36,    30,    31,    29,    28,    29,
      29,    31,    32,    93,    34,    35,    36,     3,     4,     5,
       6,     7,    47,    48,    29,    29,    29,    52,    29,    29,
      29,     3,     4,     5,     6,     7,   116,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    29,
      29,    23,    24,    25,    26,    27,     3,     4,     5,     6,
       7,    29,    29,    29,    36,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    29,    29,    23,    24,    25,    26,
      27,    29,    23,    29,    29,    29,    29,    29,    -1,    36,
      12,    13,    14,    15,    16,    17,    18,    19,    20,   112,
      33,    23,    24,    25,    26,    27,    21,    22,    -1,    -1,
      -1,    -1,    -1,    28,    36,    -1,    31,    32,    -1,    34,
      35,    36
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    21,    22,    28,    31,    32,    34,    35,    36,    38,
      39,    40,    42,    43,    44,    45,    48,    50,     3,     4,
       5,     6,     7,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    23,    24,    25,    26,    27,
      36,    36,    29,    41,    42,     0,    28,    44,    44,    44,
      28,    43,    44,    28,    43,    41,    43,    28,    48,    36,
      36,    36,    28,    44,    44,    29,    50,    43,    43,    43,
      43,    43,    43,    43,    44,    48,    49,    31,    29,    42,
      44,    44,    29,    43,    44,    43,    29,    48,    48,    43,
      28,    47,    43,    36,    49,    49,    29,    29,    43,    43,
      43,    43,    43,    29,    29,    29,    29,    29,    29,    29,
      29,    29,    36,    46,    49,    29,    43,    29,    29,    49,
      29,    29,    29,    29,    29,    46,    29,    29,    43,    29,
      29,    49,    29
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    37,    38,    38,    39,    39,    39,    40,    40,    40,
      41,    41,    42,    42,    43,    43,    43,    43,    43,    43,
      43,    43,    43,    43,    43,    43,    43,    43,    43,    43,
      43,    43,    43,    44,    44,    44,    44,    44,    44,    44,
      45,    45,    46,    46,    47,    48,    48,    48,    49,    49,
      49,    50
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     1,     1,     1,     1,     3,     2,     4,
       2,     1,     1,     1,     5,     5,     5,     5,     5,     4,
       5,     6,     1,     4,     4,     1,     1,     5,     6,     5,
       9,     5,     3,     5,     5,     4,     5,     5,     5,     1,
       1,     1,     1,     2,     3,     5,     5,     1,     1,     1,
       1,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 4: /* INPUT: EXPI  */
#line 179 ".\\gpp_interpreter.y"
                        { printEXPI((yyval.value)); }
#line 1336 "gpp_interpreter.tab.c"
    break;

  case 5: /* INPUT: EXPLISTI  */
#line 180 ".\\gpp_interpreter.y"
                        { printEXPLISTI(); }
#line 1342 "gpp_interpreter.tab.c"
    break;

  case 6: /* INPUT: EXPB  */
#line 181 ".\\gpp_interpreter.y"
                        { printEXPB((yyval.value)); }
#line 1348 "gpp_interpreter.tab.c"
    break;

  case 7: /* LISTVALUE: OP_LIST VALUES OP_CP  */
#line 184 ".\\gpp_interpreter.y"
                                            { if (idx==0 && !ERROR) { idx=indexArr; }}
#line 1354 "gpp_interpreter.tab.c"
    break;

  case 8: /* LISTVALUE: OP_LIST OP_CP  */
#line 185 ".\\gpp_interpreter.y"
                                            {(yyval.values)=0;}
#line 1360 "gpp_interpreter.tab.c"
    break;

  case 9: /* LISTVALUE: OP_OP KW_LIST VALUES OP_CP  */
#line 186 ".\\gpp_interpreter.y"
                                            {PRINTFLAG=1; LISTFLAG=1; (yyval.values)=(yyvsp[-1].values);}
#line 1366 "gpp_interpreter.tab.c"
    break;

  case 10: /* VALUES: VALUES NUMBER  */
#line 189 ".\\gpp_interpreter.y"
                            { arr[indexArr]=(yyvsp[0].value); indexArr=indexArr+1; }
#line 1372 "gpp_interpreter.tab.c"
    break;

  case 11: /* VALUES: NUMBER  */
#line 190 ".\\gpp_interpreter.y"
                            { arr[indexArr]=(yyvsp[0].value) ; indexArr=indexArr+1; }
#line 1378 "gpp_interpreter.tab.c"
    break;

  case 14: /* EXPI: OP_OP OP_PLUS EXPI EXPI OP_CP  */
#line 197 ".\\gpp_interpreter.y"
                                            {(yyval.value)=(yyvsp[-2].value)+(yyvsp[-1].value); PRINTFLAG=1; NUMFLAG=1;}
#line 1384 "gpp_interpreter.tab.c"
    break;

  case 15: /* EXPI: OP_OP OP_MINUS EXPI EXPI OP_CP  */
#line 198 ".\\gpp_interpreter.y"
                                             {(yyval.value)=(yyvsp[-2].value)-(yyvsp[-1].value); PRINTFLAG=1; NUMFLAG=1;}
#line 1390 "gpp_interpreter.tab.c"
    break;

  case 16: /* EXPI: OP_OP OP_MULT EXPI EXPI OP_CP  */
#line 199 ".\\gpp_interpreter.y"
                                            {(yyval.value)=(yyvsp[-2].value)*(yyvsp[-1].value); PRINTFLAG=1; NUMFLAG=1;}
#line 1396 "gpp_interpreter.tab.c"
    break;

  case 17: /* EXPI: OP_OP OP_DIV EXPI EXPI OP_CP  */
#line 200 ".\\gpp_interpreter.y"
                                           {if((yyvsp[-1].value) != 0){(yyval.value)=(yyvsp[-2].value)/(yyvsp[-1].value);} PRINTFLAG=1; NUMFLAG=1;}
#line 1402 "gpp_interpreter.tab.c"
    break;

  case 18: /* EXPI: OP_OP OP_POW EXPI EXPI OP_CP  */
#line 201 ".\\gpp_interpreter.y"
                                           { pow((yyvsp[-2].value),(yyvsp[-1].value)); PRINTFLAG=1; NUMFLAG=1;}
#line 1408 "gpp_interpreter.tab.c"
    break;

  case 19: /* EXPI: OP_OP IDENTIFIER EXP OP_CP  */
#line 202 ".\\gpp_interpreter.y"
                                         { (yyval.value)= (yyvsp[-1].value); LISTFLAG=1; PRINTFLAG=1;  }
#line 1414 "gpp_interpreter.tab.c"
    break;

  case 20: /* EXPI: OP_OP KW_SET IDENTIFIER EXPI OP_CP  */
#line 203 ".\\gpp_interpreter.y"
                                                 { (yyval.value) = (yyvsp[-1].value); appendToSymbolTable((yyvsp[-1].value),(yyvsp[-2].id)); NUMFLAG=1; PRINTFLAG=1;}
#line 1420 "gpp_interpreter.tab.c"
    break;

  case 21: /* EXPI: OP_OP KW_DEFFUN IDENTIFIER IDENTIFIERLIST EXP OP_CP  */
#line 204 ".\\gpp_interpreter.y"
                                                                  {(yyval.value) = 0;}
#line 1426 "gpp_interpreter.tab.c"
    break;

  case 22: /* EXPI: IDENTIFIER  */
#line 205 ".\\gpp_interpreter.y"
                         {(yyval.value) = getValue((yyvsp[0].id)); NUMFLAG=1; PRINTFLAG=1;}
#line 1432 "gpp_interpreter.tab.c"
    break;

  case 23: /* EXPI: OP_OP KW_LOAD STRING OP_CP  */
#line 206 ".\\gpp_interpreter.y"
                                         {(yyval.value)=0;}
#line 1438 "gpp_interpreter.tab.c"
    break;

  case 24: /* EXPI: OP_OP KW_DISP EXPI OP_CP  */
#line 207 ".\\gpp_interpreter.y"
                                       {(yyval.value) = (yyvsp[-1].value); NUMFLAG=1; PRINTFLAG=1;}
#line 1444 "gpp_interpreter.tab.c"
    break;

  case 25: /* EXPI: NUMBER  */
#line 208 ".\\gpp_interpreter.y"
                     {(yyval.value)=(yyvsp[0].value);}
#line 1450 "gpp_interpreter.tab.c"
    break;

  case 26: /* EXPI: STRING  */
#line 209 ".\\gpp_interpreter.y"
                     {(yyval.value) = 0;}
#line 1456 "gpp_interpreter.tab.c"
    break;

  case 27: /* EXPI: OP_OP KW_IF EXPB EXP OP_CP  */
#line 210 ".\\gpp_interpreter.y"
                                          {
                if((yyvsp[-2].value)==1)
                    (yyval.value) = (yyvsp[-1].value);
            }
#line 1465 "gpp_interpreter.tab.c"
    break;

  case 28: /* EXPI: OP_OP KW_IF EXPB EXP EXP OP_CP  */
#line 214 ".\\gpp_interpreter.y"
                                             {
                if((yyvsp[-3].value)==1)
                    (yyval.value) = (yyvsp[-2].value);
                else
                    (yyval.value) = (yyvsp[-1].value);
            }
#line 1476 "gpp_interpreter.tab.c"
    break;

  case 29: /* EXPI: OP_OP KW_WHILE EXPB EXP OP_CP  */
#line 220 ".\\gpp_interpreter.y"
                                            {(yyval.value) = 0;}
#line 1482 "gpp_interpreter.tab.c"
    break;

  case 30: /* EXPI: OP_OP KW_FOR OP_OP IDENTIFIER EXPI EXPI OP_CP EXP OP_CP  */
#line 221 ".\\gpp_interpreter.y"
                                                                      {(yyval.value) = 0;}
#line 1488 "gpp_interpreter.tab.c"
    break;

  case 31: /* EXPI: OP_OP KW_DEFVAR IDENTIFIER EXPI OP_CP  */
#line 222 ".\\gpp_interpreter.y"
                                                    {  PRINTFLAG=1; NUMFLAG=1; (yyval.value)=(yyvsp[-1].value); }
#line 1494 "gpp_interpreter.tab.c"
    break;

  case 32: /* EXPI: OP_OP KW_EXIT OP_CP  */
#line 223 ".\\gpp_interpreter.y"
                                  { EXITPROG=1; return 0;}
#line 1500 "gpp_interpreter.tab.c"
    break;

  case 33: /* EXPB: OP_OP KW_AND EXPB EXPB OP_CP  */
#line 226 ".\\gpp_interpreter.y"
                                           {(yyval.value)=((yyvsp[-2].value) && (yyvsp[-1].value));  PRINTFLAG=1;}
#line 1506 "gpp_interpreter.tab.c"
    break;

  case 34: /* EXPB: OP_OP KW_OR EXPB EXPB OP_CP  */
#line 227 ".\\gpp_interpreter.y"
                                          {(yyval.value)=((yyvsp[-2].value) || (yyvsp[-1].value)); PRINTFLAG=1; }
#line 1512 "gpp_interpreter.tab.c"
    break;

  case 35: /* EXPB: OP_OP KW_NOT EXPB OP_CP  */
#line 228 ".\\gpp_interpreter.y"
                                      {(yyval.value)=!(yyvsp[-1].value);  PRINTFLAG=1;}
#line 1518 "gpp_interpreter.tab.c"
    break;

  case 36: /* EXPB: OP_OP KW_LESS EXPI EXPI OP_CP  */
#line 229 ".\\gpp_interpreter.y"
                                            { (yyval.value)=((yyvsp[-2].value) < (yyvsp[-1].value)); PRINTFLAG=1; }
#line 1524 "gpp_interpreter.tab.c"
    break;

  case 37: /* EXPB: OP_OP KW_EQUAL EXPB EXPB OP_CP  */
#line 230 ".\\gpp_interpreter.y"
                                             {(yyval.value)=((yyvsp[-2].value)==(yyvsp[-1].value));  PRINTFLAG=1; }
#line 1530 "gpp_interpreter.tab.c"
    break;

  case 38: /* EXPB: OP_OP KW_EQUAL EXPI EXPI OP_CP  */
#line 231 ".\\gpp_interpreter.y"
                                             {(yyval.value)=((yyvsp[-2].value)==(yyvsp[-1].value));  PRINTFLAG=1; }
#line 1536 "gpp_interpreter.tab.c"
    break;

  case 40: /* BINARYVALUE: KW_TRUE  */
#line 235 ".\\gpp_interpreter.y"
                       { (yyval.value)=1;}
#line 1542 "gpp_interpreter.tab.c"
    break;

  case 41: /* BINARYVALUE: KW_FALSE  */
#line 236 ".\\gpp_interpreter.y"
                       {(yyval.value)=0; }
#line 1548 "gpp_interpreter.tab.c"
    break;

  case 45: /* EXPLISTI: OP_OP KW_CONCAT EXPLISTI EXPLISTI OP_CP  */
#line 246 ".\\gpp_interpreter.y"
                                                      {(yyval.values)=0;}
#line 1554 "gpp_interpreter.tab.c"
    break;

  case 46: /* EXPLISTI: OP_OP KW_APPEND EXPI EXPLISTI OP_CP  */
#line 247 ".\\gpp_interpreter.y"
                                                  { append(arr,indexArr,(yyvsp[-2].value)); indexArr=indexArr+1; }
#line 1560 "gpp_interpreter.tab.c"
    break;

  case 47: /* EXPLISTI: LISTVALUE  */
#line 248 ".\\gpp_interpreter.y"
                        { LISTFLAG=1; PRINTFLAG=1;}
#line 1566 "gpp_interpreter.tab.c"
    break;

  case 48: /* EXP: EXPLISTI  */
#line 251 ".\\gpp_interpreter.y"
                        {(yyval.value) = (yyvsp[0].values);}
#line 1572 "gpp_interpreter.tab.c"
    break;

  case 49: /* EXP: EXPI  */
#line 252 ".\\gpp_interpreter.y"
                        {(yyval.value) = (yyvsp[0].value);}
#line 1578 "gpp_interpreter.tab.c"
    break;

  case 50: /* EXP: EXPB  */
#line 253 ".\\gpp_interpreter.y"
                        {(yyval.value) = (yyvsp[0].value);}
#line 1584 "gpp_interpreter.tab.c"
    break;


#line 1588 "gpp_interpreter.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 259 ".\\gpp_interpreter.y"


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
