
%{
    #include <stdio.h>
    #include <string.h>
    #include <stdbool.h>
    #include <stdlib.h>
    #include "ast.h"
    #define YYSTYPE Node*
    #include "parser.tab.h"
    int line_num = 1;
%}

%%
[0-9]+\.[0-9]+  {yylval = createNode(FLOAT_NODE,yytext,NULL,NULL);return FLOAT;}
if      {return IF;}
else      {return ELSE;}
elif      {return ELIF;}
print      {return PRINT;}
printstr      {return PRINTSTR;}
var      {return VAR;}
set      {return SET;}
end      {return END;}
while {return WHILE;}
dowhile {return DOWHILE;}
and {return AND;}
or {return OR;}
[a-zA-Z_]+[0-9]* {yylval = createNode(ID_NODE,yytext,NULL,NULL);return ID;}
\+ {return ADD;}
- {return SUB;}
"(" {return LPAR;}
")" {return RPAR;}
"*" {return MUL;}
"/" {return DIV;}
% {return MOD;}
"<=" {return LTE;}
"<" {return LT;}
">=" {return GTE;}
">" {return GT;}
"==" {return EQ;}
"!=" {return NOTEQ;}
\"(.+)\" {yylval = createNode(STR_NODE,yytext,NULL,NULL);return STR;}
" "|\r|\t 
\n {line_num++;}
. {printf("Line %d: Illegal character used in program: '%s'\n",line_num,yytext);exit(1);}
%%

