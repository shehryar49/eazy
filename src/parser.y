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

%%
program: stmts {puts("valid program");};
stmts: %empty | stmt stmts;
stmt: varStmt | printStmt | setStmt | printStrStmt | ifStmt | ifElseStmt | ifElifStmt | ifElifElseStmt;
varStmt: VAR ID FLOAT;
printStmt: PRINT ID | PRINT FLOAT;
printStrStmt: PRINTSTR STR;
setStmt: SET ID FLOAT;
ifStmt: IF FLOAT stmts END;
ifElseStmt: IF FLOAT stmts ELSE stmts END;
ifElifStmt: IF FLOAT stmts ELIF FLOAT stmts END; 
ifElifElseStmt: IF FLOAT stmts ELIF FLOAT stmts ELSE stmts END;
%%

int yyerror(const char* msg)
{
    puts(msg);
    return 1;
}