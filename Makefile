all:
	g++ -I src/include -L src/lib -o main snake_sdl.cpp -lmingw32 -lSDL2main -lSDL2