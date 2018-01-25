#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "flood_lib.h"
#include "const.h"

// solve an input, and output just a sequence of datas
// will also read some command line options, to define the max size of array of solutions

int print_path(int path[MAX_PATH], int path_length) // only for debug
{
	int i;
	for (i=0;i<path_length;i++)
	{
		if (path[i]!=-1) { printf("%d", path[i]); }
	}
	printf("\n");
	return 0;
}

int main(int argc, char *argv[])
{ 
	//signal(SIGSEGV, handler);   // install our handler -> backtrace sucks

	struct parameters p;
	p.size_x=0; p.size_y=0; 
	static int paths[2*MAX_PATHS][MAX_PATH]; // longuest path will be 100
	static int board[MAX_SIZE_X][MAX_SIZE_Y];
	static int owneds[2*MAX_PATHS][MAX_SIZE_X][MAX_SIZE_Y];
	static int owned[MAX_SIZE_X][MAX_SIZE_Y]; // temp 'owned' to check if some path is worth being stored
	int coverts[2*MAX_PATHS]; // number of cells covered 
	int count_path[2]={1, 0};  // first arrays are size 1 (the initial status), second part of the arrays is size 0 
	int col;
	if (!parse_parameters(argc, argv, &p)) { exit(1); } 
	int i, j;
	// we init paths with -1, will make it easier to debug
	for (i=0;i<p.max_paths_check;i++) {
		for (j=0;j<MAX_PATH;j++) {
			paths[i][j]=-1; } } 
	init_board_from_stdin(board, &p); 

	printf("size board loaded : %d, %d\n", p.size_x, p.size_y);

	if (!test_parameters(&p)) { exit(1); } // check if parameters match the actual board size
	init_owned(owneds[0], &p);
	update_owned(board, owneds[0], board[p.begin_x][p.begin_y], &p);
	coverts[0]=get_covert(owneds[0]); 
	int win=0; 
	int index_win=0;
	int path_length=0; // length of the paths tested
	int k,l;
	int swap=0;
	int last_col;
	int last_cov; // old coverage
	int cov; // coverage, cells owned
	int min_cov; // min coverage for a section. will be used to find out which line i want to remove to make room
	int max_cov;
	int max_cov_index; // index of the minimu coverage 
	while (!win) {
		if (path_length > MAX_PATH) { 
			max_cov=-1;
			max_cov_index=0;
			for (i=0;i < p.max_paths_check; i++) 
			{
				if (coverts[(1-swap)*p.max_paths_check+i] > max_cov) {
					max_cov_index=(1-swap)*p.max_paths_check+i;
					max_cov=coverts[max_cov_index];
				}
			} 
			print_path(paths[swap*p.max_paths_check+max_cov_index], path_length); 
			fprintf(stderr, "path too long, %d, coverage of %d\n", path_length, max_cov); 
			return 3;
		}

		// we check the path with the most coverage
		max_cov=-1;
		max_cov_index=0;
		for (j=0;j < p.max_paths_check; j++) 
		{
			if (coverts[(1-swap)*p.max_paths_check+j] > max_cov) {
				max_cov_index=(1-swap)*p.max_paths_check+j;
				max_cov=coverts[max_cov_index];
			}
		} 
		// we'll refill from 0 the destination
		count_path[1-swap]=0;

		// for each existing path
		for (i=0;i<count_path[swap];i++)
		{ 
			last_col=paths[swap*p.max_paths_check+i][path_length-1];
			col=p.min_col;
			// we try each color
			while ((!win) && (col <= p.max_col)) // TODO : function
			{ 
				// if it's the first path, or we didn't change color, then we don't do anythg
				if ((path_length==0) || (col!=last_col)) 
				{
					last_cov=coverts[swap*p.max_paths_check+i];
					for (k=0;k<p.size_x;k++) {
						for (l=0;l<p.size_y;l++) {
							owned[k][l]=owneds[swap*p.max_paths_check+i][k][l];
						}
					} 
					update_owned(board, owned, col, &p);
					cov=get_covert(owned); 
					// if that new color added to the path helped
					if (cov>last_cov) { 
						// if we reached the limit
						if (count_path[1-swap]==p.max_paths_check) { // number of path for the destination 
							// lets find the worst path of the destination
							min_cov=coverts[(1-swap)*p.max_paths_check]; //TODO function to find the worst
							j=0;
							for (k=1;k<p.max_paths_check;k++) 
							{
								if (coverts[(1-swap)*p.max_paths_check+k] < min_cov) {
									min_cov=coverts[(1-swap)*p.max_paths_check+j];
									j=k;
								}
							} 
							// we'll write at the j location, the worst of the lot
						}
						else
						{
							// we'll write at a new location
							j=count_path[1-swap]; 
							count_path[1-swap]=j+1;
						} 
						// we write the new owned array
						for (k=0;k<p.size_x;k++) { // TODO function to copy array, path and coverts
							for (l=0;l<p.size_y;l++) {
								owneds[(1-swap)*p.max_paths_check+j][k][l]=owned[k][l];
							}
						} 
						// we write the new path
						for (k=0;k<path_length;k++) {
							paths[(1-swap)*p.max_paths_check+j][k]=paths[swap*p.max_paths_check+i][k]; // that seems wrong
						} 
						paths[(1-swap)*p.max_paths_check+j][path_length]=col; 
						// we write the new covert
						coverts[(1-swap)*p.max_paths_check+j]=cov;
						// check if it's a win
						if (cov==p.size_x*p.size_y) { win=1; index_win=(1-swap)*p.max_paths_check+j; }; 
					} 
				}
				col++;
			} 
		} 
		path_length+=1;
		swap=1-swap; // we swap here
	}
	print_path(paths[index_win], path_length);
	return 0;
} 
