IDIR=include
CC=gcc
CFLAGS=-Wall -std=c99 -g -I$(IDIR)
vpath %.c ./src

SOURCES=$(filter-out src/test.c, $(wildcard src/*.c))
OBJECTS=$(patsubst src/%.c,obj/%.o,$(SOURCES))
EXEC=bin/fileManager

TEST_SRC=src/test.c src/AVLTree.c
TEST_OBJ=$(patsubst src/%.c,obj/%.o,$(TEST_SRC))
TEST_EXEC=bin/treeTest

all: test program

test: $(TEST_OBJ)
	$(CC) -o $(TEST_EXEC) $(TEST_OBJ)
	
program: $(OBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS)
	
obj/%.o: %.c
	@echo $<
	@$ $(CC) $(CFLAGS) -c $< -o $@
	
clean:
	rm -f obj/*.o bin/*