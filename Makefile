
EXEC = lex


SRC = main.c lexical.c parse.c


CC = gcc
CFLAGS = -Wall -Wextra -std=c11


all: $(EXEC)


$(EXEC): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(EXEC)


run: $(EXEC)
	./$(EXEC) in.c out.txt

clean:
	rm -f $(EXEC) *.o out.txt

.PHONY: all run clean