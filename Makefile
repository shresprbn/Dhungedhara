all:
	g++ -I sdl/include -L sdl/lib -o main src/main.cpp -lmingw32 -lSDL2main -lSDL2