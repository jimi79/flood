#include "const.h"
#include "flood_type.h"

int parse_parameters(int argc, char *argv[], struct params *p);
int color_print(int x, int y, int col, struct params *p);
int print_board(int board[MAX_SIZE_X][MAX_SIZE_Y], int owned[MAX_SIZE_X][MAX_SIZE_Y], struct params *p);
int get_covert(int owned[MAX_SIZE_X][MAX_SIZE_Y]);
int update_owned_2(int board[MAX_SIZE_X][MAX_SIZE_Y], int owned[MAX_SIZE_X][MAX_SIZE_Y], int color, struct params *p);
