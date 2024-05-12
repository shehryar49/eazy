%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "ast.h"
    int yyerror(const char*);
    int yylex();
    extern char* yytext;
    #define YYSTYPE Node*
%}

%token IF
%token ELIF
%token ELSE
%token WHILE
%token DOWHILE
%token END
%token VAR
%token PRINT
%token PRINTSTR
%token SET
%token FLOAT
%token ID
%token STR
%token ADD
%token SUB
%token MUL
%token DIV
%token LTE
%token LT
%token GTE
%token GT
%token EQ
%token NOTEQ
%token MOD
%token LPAR
%token RPAR


%%
program: stmts {puts("valid program");$$ = createNode(PROGRAM_NODE,"PROGRAM",$1,NULL);prettyPrint($$);};
stmts: %empty {$$ = NULL;}| stmt stmts {$$ = createNode(STMTS_NODE,"STMTS",$1,$2);};
stmt: varStmt {$$ = createNode(STMT_NODE,"STMT",$1,NULL);}|
      printStmt {$$ = createNode(STMT_NODE,"STMT",$1,NULL);}|
      setStmt {$$ = createNode(STMT_NODE,"STMT",$1,NULL);}|
      printStrStmt {$$ = createNode(STMT_NODE,"STMT",$1,NULL);}|
      ifStmt {$$ = createNode(STMT_NODE,"STMT",$1,NULL);}|
      ifElseStmt {$$ = createNode(STMT_NODE,"STMT",$1,NULL);}|
      ifElifStmt {$$ = createNode(STMT_NODE,"STMT",$1,NULL);}|
      ifElifElseStmt {$$ = createNode(STMT_NODE,"STMT",$1,NULL);}|
      whileStmt {$$ = createNode(STMT_NODE,"STMT",$1,NULL);}|
      dowhileStmt {$$ = createNode(STMT_NODE,"STMT",$1,NULL);};

varStmt: VAR ID expr {$$ = createNode(VAR_NODE,"VAR",$2,$3);};
printStmt: PRINT expr {$$ = createNode(PRINT_NODE,"PRINT",$2,NULL);};
printStrStmt: PRINTSTR STR {$$ = createNode(PRINTSTR_NODE,"PRINTSTR",$2,NULL);};
setStmt: SET ID expr {$$ = createNode(SET_NODE,"SET",$2,NULL);};
ifStmt: IF LPAR expr RPAR stmts END {$$ = createNode(IF_NODE,"IF",$3,$5);};
ifElseStmt: IF LPAR expr RPAR stmts ELSE stmts END {$$ = createNode(IFELSE_NODE,"IFELSE",NULL,NULL);};
ifElifStmt: IF LPAR expr RPAR stmts ELIF LPAR expr RPAR stmts END {$$ = createNode(IFELIF_NODE,"IFELIF",NULL,NULL);}; 
ifElifElseStmt: IF LPAR expr RPAR stmts ELIF LPAR expr RPAR stmts ELSE stmts END {$$ = createNode(IFELIFELSE_NODE,"IFELIFELSE",NULL,NULL);};
whileStmt: WHILE LPAR expr RPAR stmts END {$$ = createNode(WHILE_NODE,"WHILE",NULL,NULL);};
dowhileStmt: DOWHILE LPAR expr RPAR stmts END {$$ = createNode(DOWHILE_NODE,"DOWHILE",NULL,NULL);};
expr: FLOAT {$$ = $1;}|
      ID {$$=$1;}|
      addExpr {$$=$1;}| 
      subExpr {$$=$1;}| 
      mulExpr {$$=$1;}| 
      divExpr {$$=$1;}|
      eqExpr  {$$=$1;}|
      noteqExpr {$$=$1;}|
      lteExpr {$$=$1;}|
      ltExpr {$$=$1;}|
      gteExpr {$$=$1;}|
      gtExpr {$$=$1;}|
      modExpr {$$=$1;};
addExpr: expr ADD FLOAT {$$ = createNode(ADD_NODE,"+",$1,$3);}| expr ADD ID {$$ = createNode(ADD_NODE,"+",$1,$3);};
subExpr: expr SUB FLOAT {$$ = createNode(SUB_NODE,"-",$1,$3);}| expr SUB ID {$$ = createNode(SUB_NODE,"-",$1,$3);};
mulExpr: expr MUL FLOAT {$$ = createNode(MUL_NODE,"*",$1,$3);}| expr MUL ID {$$ = createNode(MUL_NODE,"*",$1,$3);};
divExpr: expr DIV FLOAT {$$ = createNode(DIV_NODE,"/",$1,$3);}| expr DIV ID {$$ = createNode(DIV_NODE,"/",$1,$3);};
eqExpr: expr EQ FLOAT {$$ = createNode(EQ_NODE,"==",$1,$3);}| expr EQ ID {$$ = createNode(EQ_NODE,"==",$1,$3);};
noteqExpr: expr NOTEQ FLOAT {$$ = createNode(NOTEQ_NODE,"!=",$1,$3);}| expr NOTEQ ID {$$ = createNode(NOTEQ_NODE,"!=",$1,$3);};
ltExpr: expr LT FLOAT {$$ = createNode(LT_NODE,"<",$1,$3);}| expr LT ID {$$ = createNode(LT_NODE,"<",$1,$3);};
lteExpr: expr LTE FLOAT {$$ = createNode(LTE_NODE,"<=",$1,$3);}| expr LTE ID {$$ = createNode(LTE_NODE,"<=",$1,$3);};
gteExpr: expr GTE FLOAT {$$ = createNode(GTE_NODE,">=",$1,$3);}| expr GTE ID {$$ = createNode(GTE_NODE,">=",$1,$3);};
gtExpr: expr GT FLOAT {$$ = createNode(GT_NODE,">",$1,$3);}| expr GT ID {$$ = createNode(GT_NODE,">",$1,$3);};
modExpr: expr MOD FLOAT {$$ = createNode(MOD_NODE,"%",$1,$3);}| expr MOD ID {$$ = createNode(MOD_NODE,"%",$1,$3);};
%%

int yyerror(const char* msg)
{
    puts(msg);
    exit(1);
}