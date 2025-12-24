all: game

game: main.cpp
	g++ -o game main.cpp

clean:
	rm -f game
