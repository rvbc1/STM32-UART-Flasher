all: compile

compile:
	g++ -std=c++17 -o STM32-UART-Flasher -Wall *.cpp *.c

run:
	./main
