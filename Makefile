CC = g++
CFLAGS = -std=c++11 -Wall -Iinclude
LDFLAGS = -lncurses
SRC_DIR = src
BIN_DIR = bin
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.o,$(SOURCES))
EXECUTABLE = $(BIN_DIR)/cfs_simulator

all: dirs $(EXECUTABLE)

dirs:
	@mkdir -p $(BIN_DIR)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR)

run: all
	./$(EXECUTABLE)