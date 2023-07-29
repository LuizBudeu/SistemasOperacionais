CC = gcc
CFLAGS = -Wall -Wextra

SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Lista de arquivos de código-fonte e objetos
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

# Nome do executável
EXECUTABLE = $(BINDIR)/simulador

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR) $(BINDIR)  # Dependências para os diretórios obj e bin
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR):
	mkdir -p $(OBJDIR) 

$(BINDIR):
	mkdir -p $(BINDIR) 

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: clean
