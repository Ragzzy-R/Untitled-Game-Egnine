CC = g++ -Wall

all:
	$(CC) core/*.cpp sample/*.cpp -I./include -I/usr/include/SDL -I/usr/local/include/SDL -lSDL -lSDL_image -lSDL_ttf -lGL -lGLU -oTest 

run: all
	./Test

clean:
	@echo Cleaning up..
	@rm Test
	@echo Done.
