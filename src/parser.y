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


%%
program: ;
%%

int yyerror(const char* msg)
{
    puts(msg);
    return 1;
}