SRC:=main.cpp
OBJ:=$(SRC:.cpp=.o)

all: build run clean

build:
	clear
	g++ $(SRC) -o $(OBJ)

run:
	./$(OBJ)

clean:
	rm ./$(OBJ)

.SILENT: run build clean