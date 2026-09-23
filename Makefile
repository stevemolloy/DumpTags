CC      = clang
CFLAGS  = -Wall -Wextra -Wswitch-enum -ggdb -DJSON_SKIP_WHITESPACE
INCLUDES = -I./external/C-Simple-JSON-Parser/ -I./external/lib/

EXE    = main
EXEDIR = bin
OBJDIR = obj

SRC_DIR = src
SRCS = $(wildcard $(SRC_DIR)/*.c)

EXTERNAL_SRCS = external/C-Simple-JSON-Parser/json.c

ALL_SRCS = $(SRCS) $(EXTERNAL_SRCS)

# Map every source file to obj/<basename>.o, flattened into one directory
OBJS = $(addprefix $(OBJDIR)/,$(notdir $(ALL_SRCS:.c=.o)))

# Let make find .c files by name in either directory
VPATH = $(SRC_DIR):external/C-Simple-JSON-Parser

DEPS = $(OBJS:.o=.d)

.PHONY: all run clean

all: $(EXEDIR)/$(EXE)

$(EXEDIR)/$(EXE): $(OBJS) | $(EXEDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

$(EXEDIR):
	mkdir -p $@

$(OBJDIR):
	mkdir -p $@

run: $(EXEDIR)/$(EXE)
	./$(EXEDIR)/$(EXE)

clean:
	rm -rf $(OBJDIR) $(EXEDIR)

-include $(DEPS)
