main:
	g++ -Wall -Wextra -O2 -o what-todo.out what-todo.cpp -lncurses

run: main
	./what-todo.out
