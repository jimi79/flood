#include "const.h"
#include "flood_type.h"

int color_print(int x, int y, int col, struct parameters *p);
int get_covert(signed char owned[MAX_SIZE_X][MAX_SIZE_Y]);
int init_board_from_stdin(signed char board[MAX_SIZE_X][MAX_SIZE_Y], struct parameters *p);
int parse_parameters(int argc, char *argv[], struct parameters *p);
int print_board(signed char board[MAX_SIZE_X][MAX_SIZE_Y], signed char owned[MAX_SIZE_X][MAX_SIZE_Y], struct parameters *p);
int test_parameters(struct parameters *p);
int update_owned(signed char board[MAX_SIZE_X][MAX_SIZE_Y], signed char owned[MAX_SIZE_X][MAX_SIZE_Y], int color, struct parameters *p);
void handler(int sig);
int init_owned(signed char owned[MAX_SIZE_X][MAX_SIZE_Y], struct parameters *p);
