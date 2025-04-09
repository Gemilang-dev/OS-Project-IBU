# Menentukan kompiler yang digunakan
CC = gcc

# Opsi kompilasi
CFLAGS = -Wall -Wextra -pedantic

# Menentukan lokasi source code, object files, dan executable
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Nama executable yang akan dihasilkan
TARGET = myshell

# Mendapatkan daftar file source code (.c) di dalam folder src
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Mendapatkan daftar file object (.o) dari file source code
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Aturan untuk mengkompilasi setiap file source code menjadi file object
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Aturan untuk menggabungkan file-file object menjadi executable
$(BIN_DIR)/$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Target phony untuk menandai target yang bukan file
.PHONY: all clean

# Target default untuk mengkompilasi executable
all: $(BIN_DIR)/$(TARGET)

# Target untuk membersihkan hasil kompilasi sebelumnya
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)