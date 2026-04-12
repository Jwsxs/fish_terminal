FLAGS = -Wall -Wextra -std=c++17 -Iinclude

SRC = src
BUILD = build

# SRCS = ./src/main.cpp ./src/process_keyboard.cpp
SRCS = $(wildcard $(SRC)/*.cpp) # procura no $(SRC) todos os .cpp
# https://www.gnu.org/software/make/manual/html_node/Wildcard-Function.html

# OBJS = $(SRCS:cpp=.o)
OBJS = $(patsubst $(SRC)/%.cpp,$(BUILD)/%.o,$(SRCS)) # compila todos os $(SRC)/*.cpp em $(SRC)/*.o e manda pra $(BUILD)/
# https://www.gnu.org/software/make/manual/html_node/Text-Functions.html

BIN = $(BUILD)/main

all: clean $(BIN) run

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/%.o: $(SRC)/%.cpp | $(BUILD)
	g++ $(FLAGS) -c $< -o $@

$(BIN): $(OBJS)
	g++ $(OBJS) -o $(BIN)

run: $(BIN)
	clear
	./$(BIN)

clean:
	rm -rf $(BUILD)

.SILENT: run clean