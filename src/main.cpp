#include "ast.h"
extern "C" int yylex();
extern "C" int yyparse();
Node* ast;
int main()
{
    yyparse();
    prettyPrint(ast,0);
    deleteAST(ast);
    return 0;
}