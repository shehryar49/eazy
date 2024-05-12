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
program: stmts {puts("valid program");};
stmts: %empty | stmt stmts;
stmt: varStmt | printStmt | setStmt | printStrStmt | ifStmt | ifElseStmt | ifElifStmt | ifElifElseStmt | whileStmt | dowhileStmt;
varStmt: VAR ID FLOAT;
printStmt: PRINT expr;
printStrStmt: PRINTSTR STR;
setStmt: SET ID expr;
ifStmt: IF LPAR expr RPAR stmts END;
ifElseStmt: IF LPAR expr RPAR stmts ELSE stmts END;
ifElifStmt: IF LPAR expr RPAR stmts ELIF LPAR expr RPAR stmts END; 
ifElifElseStmt: IF LPAR expr RPAR stmts ELIF LPAR expr RPAR stmts ELSE stmts END;
whileStmt: WHILE LPAR expr RPAR stmts END;
dowhileStmt: DOWHILE LPAR expr RPAR stmts END;
expr: FLOAT | ID | addExpr | subExpr | mulExpr | divExpr |eqExpr | noteqExpr | lteExpr | ltExpr | gteExpr | gtExpr | modExpr;
addExpr: expr ADD FLOAT | expr ADD ID;
subExpr: expr SUB FLOAT | expr SUB ID;
mulExpr: expr MUL FLOAT | expr MUL ID;
divExpr: expr DIV FLOAT | expr DIV ID;
eqExpr: expr EQ FLOAT | expr EQ ID;
noteqExpr: expr NOTEQ FLOAT | expr NOTEQ ID;
ltExpr: expr LT FLOAT | expr LT ID;
lteExpr: expr LTE FLOAT | expr LTE ID;
gteExpr: expr GTE FLOAT | expr GTE ID;
gtExpr: expr GT FLOAT | expr GT ID;
modExpr: expr MOD FLOAT | expr MOD ID;
%%

int yyerror(const char* msg)
{
    puts(msg);
    return 1;
}