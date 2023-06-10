program: main.cpp
	g++ main.cpp -lpthread -lncurses -std=c++17
run: program
	./a.out
