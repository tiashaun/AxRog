CFLAGS += -ggdb -O0
OBJECTS=axrog.o tile.o room.o party.o character.o map.o game.o resource.o \
    entity.o character.o monster.o
EXTRAHEADS=geometry.hpp enums.hpp
LIBS=-lSDL -lSDL_image
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
