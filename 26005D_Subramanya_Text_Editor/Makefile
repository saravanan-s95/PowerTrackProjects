CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
SRC_DIR = src
OBJ_DIR = obj

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
TARGET = texteditor

# Default target — builds everything
all: $(TARGET)

# Link all object files into the final executable, placed in pwd
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile each .c into a matching .o inside obj/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create obj/ if it doesn't exist yet
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Remove build artifacts
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Rebuild from scratch
rebuild: clean all

.PHONY: all clean rebuild