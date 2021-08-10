all: compile

BIN_DIR=./bin

compile:
	[ -d $(BIN_DIR) ] || mkdir -p $(BIN_DIR)
	g++ -std=c++17 -o $(BIN_DIR)/STM32-UART-Flasher -Wall *.cpp *.c

run:
	./main
