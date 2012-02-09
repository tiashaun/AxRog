CFLAGS += -ggdb -O0

axrog: axrog.c
	$(CC) -o $@ axrog.c $(LFLAGS) $(CFLAGS) -lSDL -lSDL_ttf -lSDL_image

clean:
	-rm -f $(OBJECTS) axrog

.PHONY: clean
