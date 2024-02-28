CC=clang
CFLAGS=-Wall -pedantic -std=c18 -ggdb -DJSON_SKIP_WHITESPACE
INCLUDES=-I./external/C-Simple-JSON-Parser/ -I./external/lib/

EXE=main
EXEDIR=bin
OBJDIR=obj

$(EXEDIR)/$(EXE): $(OBJDIR)/main.o $(OBJDIR)/json.o $(OBJDIR)/lib.o
	@mkdir -p $(EXEDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

$(OBJDIR)/main.o: src/main.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $^ -o $@

$(OBJDIR)/lib.o: external/lib/lib.c
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

