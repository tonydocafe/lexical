
EXEC = lex


SRC = main.c lexical.c


CC = gcc
CFLAGS = -Wall -Wextra -std=c11


all: $(EXEC)


$(EXEC): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(EXEC)


run: $(EXEC)
	./$(EXEC) in.c out.c

clean:
	rm -f $(EXEC) *.o out.c

.PHONY: all run clean