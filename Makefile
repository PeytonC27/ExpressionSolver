test: DExpression.o main.o
	g++ -o test DExpression.o main.o -Wall

main.o: main.cpp
	g++ -c main.cpp -Wall

DExpression.o: DExpression.cpp
	g++ -c DExpression.cpp -Wall