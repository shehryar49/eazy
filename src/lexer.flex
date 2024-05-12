
%{
    #include <stdio.h>
    #include <string.h>
    #include <stdbool.h>
    #include <stdlib.h>
    #include "parser.tab.h"
%}

%%
[0-9]+\.[0-9]+  {return FLOAT;}
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
[a-zA-Z_]+[0-9]* {return ID;}
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
\"(.+)\" {return STR;}
.|\r|\n {}

%%

