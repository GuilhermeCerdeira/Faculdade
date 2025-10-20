CC = gcc
CFLAGS = -Wall -g -DYYDEBUG=1
LDFLAGS = -lfl
TARGET = tradutor
LEX_SRC = tradutor.l
BISON_SRC = tradutor.y
GEN_C_SRC = tradutor.tab.c
GEN_LEX_C_SRC = lex.yy.c
GEN_HEADER = tradutor.tab.h
all: $(TARGET)
$(TARGET): $(GEN_C_SRC) $(GEN_LEX_C_SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(GEN_C_SRC) $(GEN_LEX_C_SRC) $(LDFLAGS)
	@echo "Executavel '$(TARGET)' criado com sucesso!"

$(GEN_C_SRC) $(GEN_HEADER): $(BISON_SRC)
	bison -d $(BISON_SRC)

$(GEN_LEX_C_SRC): $(LEX_SRC) $(GEN_HEADER)
	flex $(LEX_SRC)

clean:
	rm -f $(TARGET) $(GEN_C_SRC) $(GEN_LEX_C_SRC) $(GEN_HEADER)
	@echo "Arquivos gerados foram removidos."

.PHONY: all clean