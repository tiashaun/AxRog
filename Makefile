CFLAGS += -ggdb -O0
OBJECTS=axrog.o party.o character.o map.o room.o tile.o game.o resource.o
LIBS=-lSDL -lSDL_image -lSDL_ttf -lm
CC=g++

axrog: ${OBJECTS}
	${CC} ${CFLAGS} -o $@ ${OBJECTS} ${LIBS}

#Objects with a header file
%.o: %.cpp %.hpp
	${CC} ${CFLAGS} -c -o $@ $<

#Objects without a header file
%.o: %.cpp
	${CC} ${CFLAGS} -c -o $@ $<

clean:
	-rm -f $(OBJECTS) axrog

.PHONY: clean
