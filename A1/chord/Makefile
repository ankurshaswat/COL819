SRC = utils.cpp coordinator.cpp finger_table.cpp node.cpp main.cpp

all: $(SRC)
	g++ -o main.out $(SRC) -I.

debug: $(SRC)
	g++ -Wall -Wextra -D_DEBUG -o main.out -g $(SRC) -I.

clean:
	rm -f *.out