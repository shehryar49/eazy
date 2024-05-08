
%{
    #include <stdio.h>
    #include <string.h>
    #include <stdbool.h>
    #include <stdlib.h>
    #include "token.h"
    #include <vector>
    #include <string>

    std::vector<Token> tokens;

    const char* tokentypenames[] = {
        "VALUE",
        "KEYWORD",
        "STRING",
        "ID"
    };
    extern "C" int yylex(void);
%}

%%
[0-9]+  {tokens.push_back(Token(VALUE,yytext));}
if      {tokens.push_back(Token(KEYWORD,yytext));}
else      {tokens.push_back(Token(KEYWORD,yytext));}
elif      {tokens.push_back(Token(KEYWORD,yytext));}
print      {tokens.push_back(Token(KEYWORD,yytext));}
printstr      {tokens.push_back(Token(KEYWORD,yytext));}
var      {tokens.push_back(Token(KEYWORD,yytext));}
[a-zA-Z_]+[0-9]* {tokens.push_back(Token(ID,yytext));}
\"(.+)\" {tokens.push_back(Token(STRING,yytext));}
.|\n|\r {}

%%

/*** C Code section ***/

int main(void)
{
    /* Call the lexer, then quit. */
    yylex();
    puts("-------------");
    for(auto token: tokens)
    {
        printf("%s: %s\n",tokentypenames[token.type],token.content.c_str());
    }
    return 0;
}