JCC = javac
JVM = java

SRC_DIR = src
BIN_DIR = bin
LIB_DIR = lib

LIBS = $(wildcard $(LIB_DIR)/*.jar)
CP_LIBS = $(subst $(eval) ,:,$(LIBS))

CP = "$(BIN_DIR):$(CP_LIBS)"

SOURCES = $(shell find $(SRC_DIR) -name "*.java")
all: compile

compile:
	@echo "Compilando fontes..."
	$(JCC) -d $(BIN_DIR) -cp "$(CP_LIBS)" $(SOURCES)

server: compile
	mkdir -p data
	$(JVM) -cp $(CP) br.com.guilhermando.servidor.server

client: compile
	$(JVM) -cp $(CP) br.com.guilhermando.cliente.client

.PHONY: clean
clean:
	@echo "Limpando diretório de compilação..."
	rm -rf $(BIN_DIR)
	rm -rf data