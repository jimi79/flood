BIN=./bin
SOURCE=./
CFLAGS=-ggdb
LIBS=flood_lib.c 

LIST=$(BIN)/flood_solve $(BIN)/flood_create $(BIN)/flood_print

all: $(LIST)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN)/%: $(SOURCE)%.c
	$(CC) $(INC) $< $(CFLAGS) -o $@ $(LIBS)

