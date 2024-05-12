all:
	flex -o src/lex.yy.c src/lexer.flex
	bison -d -v src/parser.y -o src/parser.tab.c
	gcc -c src/lex.yy.c -g
	gcc -c src/parser.tab.c -g
	g++ -c src/ast.cpp -g
	g++ -c src/main.cpp -g
	g++ lex.yy.o main.o parser.tab.o ast.o -o eazy -lfl -g
clean:
	rm -f *.o
	rm -f src/lex.yy.c
	rm -f src/parser.tab.h
	rm -f src/parser.tab.c
	rm -f src/parser.output
	rm -f eazy
