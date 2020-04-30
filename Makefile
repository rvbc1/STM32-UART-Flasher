all: compile run

compile:
	g++ -o main -Wall *.cpp *.c

run:
	./main
