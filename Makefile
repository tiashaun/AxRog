OBJECTS = axrog.o

all: axrog

axrog: axrog.o
	$(CC) $(LFLAGS) $(OBJECTS) -o $@ -lSDL -lSDL_ttf -lSDL_image -ggdb

clean:
	-rm -f $(OBJECTS) axrog

.PHONY: all clean
