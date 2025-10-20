calculator: trab.y trab.l functions.c
	bison -d trab.y
	flex -o trab.lex.c trab.l
	gcc -o $@ trab.tab.c trab.lex.c functions.c -lm

	rm -f trab.tab.c
	rm -f trab.tab.h
	rm -f trab.lex.c

clear:
	rm -f calculator