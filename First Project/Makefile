CC = gcc
CFLAGS = -Wall -Iinclude       # Menambahkan -Iinclude untuk mencari file header di folder include/
SRC = src/shell.c src/commands.c src/utils.c
OBJ = $(SRC:.c=.o)
EXEC = bin/shell

$(EXEC): $(OBJ)
	$(CC) -o $(EXEC) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)
