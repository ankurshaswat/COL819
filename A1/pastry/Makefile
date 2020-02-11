SRC = leaf_set.cpp md5.cpp neighbourhood_set.cpp routing_table.cpp utils.cpp node.cpp coordinator.cpp main.cpp

all: $(SRC)
	g++ -o main.out $(SRC) -I.

debug: $(SRC)
	g++ -Wall -Wextra -D_DEBUG -o main.out -g $(SRC) -I.

clean:
	rm -f *.out