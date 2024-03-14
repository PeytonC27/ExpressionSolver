
test: main.o expression.o
	g++ -o test main.o expression.o -Wall

main.o: main.cpp
	g++ -c main.cpp -Wall

expression.o: expression.cpp
	g++ -c expression.cpp -Wall