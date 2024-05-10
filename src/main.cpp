
extern "C" int yylex();
extern "C" int yyparse();
int main()
{
    yyparse();
    return 0;
}