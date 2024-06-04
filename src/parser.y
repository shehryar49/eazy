%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "ast.h"
    int yyerror(const char*);
    int yylex();
    extern char* yytext;
    extern Node* ast;
    extern int line_num;
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
%token AND
%token OR

%left AND OR
%left EQ NOTEQ LTE LT GT GTE
%left SUB ADD
%left DIV MUL MOD


%%
program: stmts {$$ = createNode(PROGRAM_NODE,"PROGRAM",$1,NULL);ast = $$;};
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
setStmt: SET ID expr {$$ = createNode(SET_NODE,"SET",$2,$3);};
ifStmt: IF LPAR expr RPAR stmts END {$$ = createNode(IF_NODE,"IF",$3,$5);};
ifElseStmt: IF LPAR expr RPAR stmts ELSE stmts END {$$ = createNode(IFELSE_NODE,"IFELSE",$3,$5);addChild($$,$7);};
ifElifStmt: IF LPAR expr RPAR stmts ELIF LPAR expr RPAR stmts END {$$ = createNode(IFELIF_NODE,"IFELIF",$3,$5);addChild($$,$8);addChild($$,$10);}; 
ifElifElseStmt: IF LPAR expr RPAR stmts ELIF LPAR expr RPAR stmts ELSE stmts END {$$ = createNode(IFELIFELSE_NODE,"IFELIFELSE",$3,$5);addChild($$,$8);addChild($$,$10);addChild($$,$12);};
whileStmt: WHILE LPAR expr RPAR stmts END {$$ = createNode(WHILE_NODE,"WHILE",$3,$5);};
dowhileStmt: DOWHILE LPAR expr RPAR stmts END {$$ = createNode(DOWHILE_NODE,"DOWHILE",$3,$5);};
expr: ID {$$=$1;}|
      FLOAT {$$=$1;}|
      divExpr {$$=$1;} |
      addExpr {$$=$1;}| 
      subExpr {$$=$1;}| 
      mulExpr {$$=$1;}| 
      eqExpr  {$$=$1;}|
      noteqExpr {$$=$1;}|
      lteExpr {$$=$1;}|
      ltExpr {$$=$1;}|
      gteExpr {$$=$1;}|
      gtExpr {$$=$1;}|
      modExpr {$$=$1;}; |
      andExpr {$$=$1;}; |
      orExpr {$$=$1;};  |
      LPAR expr RPAR { $$ = $2; };
      
addExpr: expr ADD expr {$$ = createNode(ADD_NODE,"+",$1,$3);};
divExpr: expr DIV expr {$$ = createNode(DIV_NODE,"/",$1,$3);};
mulExpr: expr MUL expr {$$ = createNode(MUL_NODE,"*",$1,$3);};
subExpr: expr SUB expr {$$ = createNode(SUB_NODE,"-",$1,$3);};
ltExpr: expr LT expr {$$ = createNode(LT_NODE,"<",$1,$3);};
lteExpr: expr LTE expr {$$ = createNode(LTE_NODE,"<=",$1,$3);};
gtExpr: expr GT expr {$$ = createNode(GT_NODE,">",$1,$3);};
gteExpr: expr GTE expr {$$ = createNode(GTE_NODE,">=",$1,$3);};
eqExpr: expr EQ expr {$$ = createNode(EQ_NODE,"==",$1,$3);};
noteqExpr: expr NOTEQ expr {$$ = createNode(NOTEQ_NODE,"!=",$1,$3);};
modExpr: expr MOD expr {$$ = createNode(MOD_NODE,"%",$1,$3);};
andExpr: expr AND expr {$$ = createNode(AND_NODE,"and",$1,$3);};
orExpr: expr OR expr {$$ = createNode(OR_NODE,"or",$1,$3);};

%%

int yyerror(const char* msg)
{
    printf("Line %d\n",line_num);
    puts(msg);
    exit(0);
}