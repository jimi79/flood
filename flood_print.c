#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "flood_lib.h"
#include "const.h"

// solve an input, and output just a sequence of datas
// will also read some command line options, to define the max size of array of solutions

int init_print_coverage(int height_board) {
	int row=height_board+2;
	printf("\033[0m\033[%d;%dHcolor:", row, 0);
	printf("\033[0m\033[%d;%dHcoverage:", row+1, 0);
}

int print_coverage(int height_board, int step, int color, int coverage) { // step is the nth step in the process
	int row=height_board+2;
	int col=step*4+10; 
	printf("\033[0m\033[%d;%dH%d", row, col, color);
	printf("\033[0m\033[%d;%dH%d", row+1, col, coverage);
}

int update_owned_color(int board[MAX_SIZE_X][MAX_SIZE_Y], int owned[MAX_SIZE_X][MAX_SIZE_Y], struct parameters *p, int live_print) {
	// for each pass, we count what we update
	// at each pass, we look if it's owned. then for 8 directions, we look if not owned, and same color. if so, direction is owned
	int updated=1; // wrong value just to start the loop
	int dir; // direction counter, we got 4 directions to check
	int i,j; // to go through the array
	int ni,nj; // new values to compare with
	int shifti, shiftj; // value to shift to check the neighbour

	while (updated > 0) { 
		updated=0;
		for (i=0; i<p->size_x;i++) {
			for (j=0; j<p->size_y;j++) {
				// we check only if we own the first cell
				if (owned[i][j]==1) {
					for (dir=0;dir<4;dir++) { 
						if (dir==0) { shifti=-1;shiftj=0; }
						if (dir==1) { shifti=1;shiftj=0; }
						if (dir==2) { shifti=0;shiftj=-1; }
						if (dir==3) { shifti=0;shiftj=1; } 
						ni=i+shifti; nj=j+shiftj;
						if ((ni>=0) && (nj>=0) && (ni<p->size_x) && (nj<p->size_y)) {
							if (owned[ni][nj]==0) {
								if (board[i][j]==board[ni][nj]) {
									if (live_print) {
										color_print(ni, nj, board[i][j], p);
									}
									owned[ni][nj]=1;
									updated+=1;
								}
							}
						}
					}
				}
			}
		} 
	} 
	return 0;
}

int update_color(int board[MAX_SIZE_X][MAX_SIZE_Y], int owned[MAX_SIZE_X][MAX_SIZE_Y], int col, struct parameters *p, int live_print) {
	int i,j;
	for (i=0;i<p->size_x;i++) {
		for (j=0;j<p->size_y;j++) { 
			if (owned[i][j]) {
				board[i][j]=col; 
				if (live_print)
				{
					color_print(i, j, col, p);
				}
			}
		} 
	} 
	return 0;
} 

int main(int argc, char *argv[]) { 
	FILE *in = stdin; 

	int line; // number of the line currently read
	int bufsize; 
	char *buffer;
	buffer=malloc(1);
	static int path[MAX_PATH]; // longuest path will be 100 (do not use signed char, it's slower)
	static int board[MAX_SIZE_X][MAX_SIZE_Y];
	static int owned[MAX_SIZE_X][MAX_SIZE_Y]; int path_length, col, max_col;
	struct parameters p; 
	int i,j; i=0; j=0;
	int end=0;
	if (!parse_parameters(argc, argv, &p)) { exit(1); }
	init_board_from_stdin(board, &p); 
	init_owned(owned, &p); 
	update_owned(board, owned, board[p.begin_x][p.begin_y], &p); 
	printf("x:%d/%d, y:%d/%d\n", p.begin_x, p.size_x, p.begin_y, p.size_y);
	if (!test_parameters(&p)) { exit(1); } 
	owned[p.begin_x][p.begin_y]=1;
	col=board[p.begin_x][p.begin_y];
	update_color(board, owned, col, &p, 0);
	update_owned_color(board, owned, &p, 0);
	char c[2];
	c[1]='\0'; 
	c[0]=p.path[3];
	printf("\033[2J\033[1;1H"); // clear screen
	if (p.display_stats) { init_print_coverage(p.size_y); }
	print_board(board, owned, &p); // doesn't locate the cursor
	for (i=0;i<strlen(p.path);i++)
	{
		c[0]=p.path[i];
		col=(int) strtol(c, NULL, 10);
		update_color(board, owned, col, &p, 1);
		update_owned_color(board, owned, &p, 1);
		if (p.display_stats) {
			print_coverage(p.size_y, i, col, get_covert(owned));
		}
		printf("\033[%d;%dH", p.size_y+1, 0);
		fflush(stdout);
		usleep(300000);
	}
	printf("\033[0m\n");

}
