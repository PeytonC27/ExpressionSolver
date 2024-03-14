main: DExpression.o main.o
	g++ -o main DExpression.o main.o -Wall

main.o: main.cpp
	g++ -c main.cpp -Wall

DExpression.o: DExpression.cpp
	g++ -c DExpression.cpp -Wall

clean:
	rm *.o main