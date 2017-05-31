#include "const.h"

int parse_parameters(int argc, char *argv[], int *begin_x, int *begin_y);
int colorize(int x, int y, short int col); 
int print_board(short int board[max_size_x][max_size_y], short int owned[max_size_x][max_size_y], int size_x, int size_y); 
int update(short int board[max_size_x][max_size_y], short int owned[max_size_x][max_size_y], int size_x, int size_y, short int live_print);
int change_color(short int board[max_size_x][max_size_y], short int owned[max_size_x][max_size_y], short int col, int size_x, int size_y, short int live_print);
