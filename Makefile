all: build

build:
	mkdir build
	g++ -std=c++17 -g -Wall -O0  ./src/*.cpp ./src/Physics/*.cpp -lm -lSDL2 -lSDL2_image -lSDL2_gfx -o build/app

run:
	./app

clean:
	rm build/app

.PHONY: all build run clean
