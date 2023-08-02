# CC = gcc
# CFLAGS = -Wall -Wextra

# SRCDIR = src
# OBJDIR = obj
# BINDIR = bin

# # Lista de arquivos de código-fonte e objetos
# SOURCES = $(wildcard $(SRCDIR)/*.c)
# SOURCES := $(filter-out src/prompt.c, $(SOURCES))
# SOURCES := $(filter-out src/prompt.exe, $(SOURCES))
# OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

# # Nome do executável
# EXECUTABLE = $(BINDIR)/simulador

# all: $(EXECUTABLE)

# $(EXECUTABLE): $(OBJECTS)
# 	$(CC) $(CFLAGS) -o $@ $^

# $(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR) $(BINDIR)  # Dependências para os diretórios obj e bin
# 	$(CC) $(CFLAGS) -c -o $@ $<

# $(OBJDIR):
# 	mkdir -p $(OBJDIR) 

# $(BINDIR):
# 	mkdir -p $(BINDIR) 

# clean:
# 	rm -rf $(OBJDIR) $(BINDIR)

# .PHONY: clean


CC = gcc
CFLAGS = -Wall -Wextra -std=c99

SIMULADOR_SOURCES = src/memoria.c src/processo.c src/main.c
PROMPT_SOURCES = src/prompt.c

SIMULADOR_OBJECTS = $(patsubst src/%.c,obj/%.o,$(SIMULADOR_SOURCES))
PROMPT_OBJECTS = $(patsubst src/%.c,obj/%.o,$(PROMPT_SOURCES))

BIN_DIR = bin
OBJ_DIR = obj

all: simulador prompt

simulador: $(SIMULADOR_OBJECTS)
	$(CC) $(CFLAGS) $(SIMULADOR_OBJECTS) -o $(BIN_DIR)/simulador

prompt: $(PROMPT_OBJECTS)
	$(CC) $(CFLAGS) $(PROMPT_OBJECTS) -o $(BIN_DIR)/prompt

$(OBJ_DIR)/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(BIN_DIR)/simulador $(BIN_DIR)/prompt $(SIMULADOR_OBJECTS) $(PROMPT_OBJECTS)