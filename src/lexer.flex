
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
[a-zA-Z_]+[0-9]* {return ID;}
\"(.+)\" {return STR;}
.|\n|\r {}

%%

