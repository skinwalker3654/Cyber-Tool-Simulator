BIN = bin
TARGET = $(BIN)/main
SRC_CODE = src/main.c

CC = gcc

all: $(TARGET)

$(BIN): 
	@echo "creating bin folder..."
	mkdir -p $(BIN)

$(TARGET): $(SRC_CODE) | $(BIN)
	@echo "Compiling the program..."
	$(CC) $< -o $@
	
run:
	@echo "Running the program..."
	./$(TARGET)

clean:
	@echo "Deleting executable file"
	rm -rf $(BIN)

.PHONY: all run clean
