CC= gcc
CFLAGS= -Werror -Wextra -Wall -pedantic -std=c99 -g

SRC= src/main.c src/help/help.c src/parse/parse.c src/makefile/makefile.c src/execution/execution.c
BIN= minimake

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $(BIN) $(SRC)

clean:
	$(RM) $(BIN)

.PHONY: all clean
