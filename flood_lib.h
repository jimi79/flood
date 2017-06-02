#include "const.h"

int parse_parameters(int argc, char *argv[], int *begin_x, int *begin_y, int *max_paths_check);
int color_print(int x, int y, int col); 
int print_board(int board[MAX_SIZE_X][MAX_SIZE_Y], int owned[MAX_SIZE_X][MAX_SIZE_Y], int size_x, int size_y); 
int get_covert(int owned[MAX_SIZE_X][MAX_SIZE_Y]);
int update_owned_2(int board[MAX_SIZE_X][MAX_SIZE_Y], int owned[MAX_SIZE_X][MAX_SIZE_Y], int color, int size_x, int size_y);
