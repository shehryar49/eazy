%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    int yyerror(const char*);
    int yylex();
%}

%token IF
%token ELIF
%token ELSE
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
program: stmts {puts("valid program");};
stmts: %empty | stmt stmts;
stmt: varStmt | printStmt | setStmt | printStrStmt | ifStmt | ifElseStmt | ifElifStmt | ifElifElseStmt;
varStmt: VAR ID FLOAT;
printStmt: PRINT ID | PRINT FLOAT;
printStrStmt: PRINTSTR STR;
setStmt: SET ID FLOAT;
ifStmt: IF LPAR expr RPAR stmts END;
ifElseStmt: IF FLOAT stmts ELSE stmts END;
ifElifStmt: IF FLOAT stmts ELIF FLOAT stmts END; 
ifElifElseStmt: IF FLOAT stmts ELIF FLOAT stmts ELSE stmts END;
expr: FLOAT | ID | addExpr | subExpr | mulExpr | divExpr |eqExpr | noteqExpr | lteExpr | ltExpr | gteExpr | gtExpr | modExpr;
addExpr: FLOAT ADD expr | ID ADD expr;
subExpr: FLOAT SUB expr | ID SUB expr;
mulExpr: FLOAT MUL expr | ID MUL expr;
divExpr: FLOAT DIV expr | ID DIV expr;
modExpr: FLOAT MOD expr | ID MOD expr;
eqExpr: FLOAT EQ expr | ID EQ expr;
noteqExpr: FLOAT NOTEQ expr | ID NOTEQ expr;
ltExpr: FLOAT LT expr | ID LT expr;
lteExpr: FLOAT LTE expr | ID LTE expr;
gtExpr: FLOAT GT expr | ID GT expr;
gteExpr: FLOAT GTE expr | ID GTE expr;


%%

int yyerror(const char* msg)
{
    puts(msg);
    return 1;
}