beastmake:
ifeq ($(OS),Windows_NT)
	gcc -o beast.exe beast.c -I /c/msys64/usr/lib/sdl2/x86_64-w64-mingw32/include/SDL2 -L /c/msys64/usr/lib/sdl2/x86_64-w64-mingw32/lib -lmingw32 -lSDL2main -lSDL2
else
	gcc -o beast beast.c -L/usr/local/lib -lSDL2
endif

beastdebug:
	gcc -g -o beast beast.c -L/usr/local/lib -lSDL2