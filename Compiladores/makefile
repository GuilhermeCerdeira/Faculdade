EXEC = calc
FLEX_SRC = calc.l
BISON_SRC = calc.y
FLEX_OUT = lex.yy.c
BISON_OUT_C = calc.tab.c
BISON_OUT_H = calc.tab.h

CC = gcc
CFLAGS = -Wall -g

all: $(EXEC)

$(EXEC): $(FLEX_OUT) $(BISON_OUT_C)
	$(CC) $(CFLAGS) $(FLEX_OUT) $(BISON_OUT_C) -lm -o $(EXEC) 

$(FLEX_OUT): $(FLEX_SRC) $(BISON_OUT_H)
	flex $(FLEX_SRC)

$(BISON_OUT_C) $(BISON_OUT_H): $(BISON_SRC)
	bison -d $(BISON_SRC)

clean:
	rm -f $(EXEC) $(FLEX_OUT) $(BISON_OUT_C) $(BISON_OUT_H)

mrproper: clean