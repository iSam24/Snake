all:
	g++ -I SDL/include -I SDL/include/SDL2 -L SDL/lib -o snakegame snake_sdl.cpp -lmingw32 -lSDL2main -lSDL2_ttf -lSDL2 -lgdi32 -lusp10 -lrpcrt4
