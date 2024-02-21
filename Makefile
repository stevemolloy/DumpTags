CC=clang
CFLAGS=-Wall -pedantic -std=c18 -ggdb
EXE=main
EXEDIR=bin
OBJDIR=obj

$(EXEDIR)/$(EXE): $(OBJDIR)/main.o $(OBJDIR)/json.o
	mkdir -p $(EXEDIR)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/main.o: src/main.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJDIR)/json.o: external/C-Simple-JSON-Parser/json.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $^ -o $@

run: $(EXE)
	./$(EXE)

clean:
	rm -rf $(OBJDIR)
	rm -rf $(EXEDIR)

