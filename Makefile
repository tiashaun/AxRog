OBJECTS = axrog.o

all: axrog
	$(CC) $(LFLAGS) $(OBJECTS) -o $@ -lSDL -lSDL_ttf -lSDL_image -ggdb -Wall -lm

axrog: axrog.o
	$(CC) $(LFLAGS) $(OBJECTS) -o $@ -lSDL -lSDL_ttf -lSDL_image -ggdb -Wall -lm

clean:
	-rm -f $(OBJECTS) axrog

.PHONY: all clean
