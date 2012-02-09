axrog: axrog.c
	$(CC) -o $@ axrog.c $(LFLAGS) -lSDL -lSDL_ttf -lSDL_image -ggdb 

clean:
	-rm -f $(OBJECTS) axrog

.PHONY: clean
