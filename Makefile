CFLAGS += -ggdb -O0 --std=c++0x
OBJECTS=axrog.o tile.o room.o party.o character.o map.o game.o resource.o
EXTRAHEADS=geometry.hpp enums.hpp
LIBS=-lSDL -lSDL_image -lSDL_ttf -lm
CC=g++

axrog: ${OBJECTS}
	${CC} ${CFLAGS} -o $@ ${OBJECTS} ${LIBS}

#Objects with a header file
%.o: %.cpp %.hpp ${EXTRAHEADS}
	${CC} ${CFLAGS} -c -o $@ $<

#Objects without a header file
%.o: %.cpp
	${CC} ${CFLAGS} -c -o $@ $<

clean:
	-rm -f $(OBJECTS) axrog

.PHONY: clean
