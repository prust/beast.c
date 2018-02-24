beastmake:
	gcc -o beast beast.c -L/usr/local/lib -lSDL2
beastdebug:
	gcc -g -o beast beast.c -L/usr/local/lib -lSDL2