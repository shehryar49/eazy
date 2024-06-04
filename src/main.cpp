
#include "ast.h"
#include "codegen.h"


extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE* yyin;

Node *ast;



int main(int argc,const char* argv[]) 
{
	if(argc != 2)
	{
		puts("usage: ./eazy filename");
		return 0;
	}
	yyin = fopen(argv[1],"r");
	if(!yyin)
	{
		perror("fopen: ");
		return 0;
	}
	yyparse();
	fclose(yyin);
	//prettyPrint( ast, 0 ); // uncomment this to print ast
	init_codegen();
	program_codegen(ast);
	deleteAST( ast );
	system("lli tmp.ll");
	return 0;
}