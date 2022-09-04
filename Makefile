build:
	gcc -Wall -std=c99 ./src/*.c -lSDL2 -o renderer 

detailedbuild:
	gcc -Wall -std=c99 ./src/*.c -o renderer -I /usr/local/Cellar/sdl2/2.24.0/include -L /usr/local/Cellar/sdl2/2.24.0/lib  -l SDL2

run:
	./renderer

clean:
	rm renderer