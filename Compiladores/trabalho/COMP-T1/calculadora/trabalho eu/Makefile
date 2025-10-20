bison-calc: calc_parser.tab.c calc_parser.tab.h calc_lexer.lex.c calc_aux.c
	gcc -o $@ calc_parser.tab.c calc_lexer.lex.c calc_aux.c -lm

calc_parser.tab.c calc_parser.tab.h: calc_parser.y
	bison -d calc_parser.y

calc_lexer.lex.c: calc_lexer.l calc_parser.tab.h
	flex -o calc_lexer.lex.c calc_lexer.l

clean:
	rm -f bison-calc calc_parser.tab.* calc_lexer.lex.c