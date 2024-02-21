CC=clang
CFLAGS=-Wall -pedantic -std=c18 -ggdb -DJSON_SKIP_WHITESPACE
INCLUDES=-I./external/C-Simple-JSON-Parser/

EXE=main
EXEDIR=bin
OBJDIR=obj

$(EXEDIR)/$(EXE): $(OBJDIR)/main.o $(OBJDIR)/json.o
	@mkdir -p $(EXEDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

$(OBJDIR)/main.o: src/main.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $^ -o $@

$(OBJDIR)/json.o: external/C-Simple-JSON-Parser/json.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $^ -o $@

run: $(EXEDIR)/$(EXE)
	./$(EXEDIR)/$(EXE)

clean:
	rm -rf $(OBJDIR)
	rm -rf $(EXEDIR)

