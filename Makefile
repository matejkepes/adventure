
# code details

EXE_DIR = .
EXE = $(EXE_DIR)/src/adventure

SRC= ./src/main.c ./src/parser.c ./src/game.c ./src/world.c ./src/room.c ./src/item.c ./src/command.c ./src/container.c ./src/backpack.c 

DEP= ./src/parser.c ./src/game.c ./src/world.c ./src/room.c ./src/item.c ./src/command.c ./src/container.c ./src/backpack.c

# generic build details

CC=      gcc
COPT=    -g

# compile to  object code

OBJ= $(SRC:.c=.o)

.c.o:
	$(CC) $(COPT) -c -o $@ $<

# build executable

$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $(EXE) 

# clean up compilation

clean:
	rm -f $(OBJ) $(EXE)

# dependencies

main.o:      main.c
parser.o:    parser.c
game.o:      game.c
world.o:     world.c
room.o:      room.c
item.o:      item.c
command.o:   command.c
container.o: container.c
backpack.o:  backpack.c