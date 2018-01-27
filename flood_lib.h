#include "const.h"
#include "flood_type.h"

int color_print(int x, int y, int col, struct parameters *p);
int get_covert(int owned[MAX_SIZE_X][MAX_SIZE_Y]);
int init_board_from_stdin(int board[MAX_SIZE_X][MAX_SIZE_Y], struct parameters *p);
int init_owned(int owned[MAX_SIZE_X][MAX_SIZE_Y], struct parameters *p);
int parse_parameters(int argc, char *argv[], struct parameters *p);
int print_board(int board[MAX_SIZE_X][MAX_SIZE_Y], int owned[MAX_SIZE_X][MAX_SIZE_Y], struct parameters *p);
int print_path(int path[MAX_PATH], int path_length);
int test_parameters(struct parameters *p);
int update_owned(int board[MAX_SIZE_X][MAX_SIZE_Y], int owned[MAX_SIZE_X][MAX_SIZE_Y], int color, struct parameters *p);
void handler(int sig);
