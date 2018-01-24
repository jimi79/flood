BIN=./
SOURCE=./
CFLAGS=-ggdb -rdynamic -funwind-tables
LIBS=flood_lib.c 

LIST=flood_solve flood_create flood_print flood_print_coverage tests

all: $(LIST)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN)/%: $(SOURCE)%.c
	$(CC) $(INC) $< $(CFLAGS) -o $@ $(LIBS)

