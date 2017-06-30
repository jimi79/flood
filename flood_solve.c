#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	FILE *in = stdin; 
	int bufsize; 
	char *buffer;
	buffer=malloc(1);
	int i=0,j=0;
	int min_col=9; // min col number
	int max_col=0; // max col number

	struct params p;
	p.size_x=0; p.size_y=0; p.begin_x=0; p.begin_x=0;
	p.max_paths_check=100; p.display_color_number=1; p.display_star=0; p.display_stat=0; 
	static int paths[2*MAX_PATHS][MAX_PATH]; // longuest path will be 100
	static int board[MAX_SIZE_X][MAX_SIZE_Y];
	static int owneds[2*MAX_PATHS][MAX_SIZE_X][MAX_SIZE_Y];
	static int owned[MAX_SIZE_X][MAX_SIZE_Y]; // temp 'owned' to check if some path is worth being stored
	int coverts[2*MAX_PATHS]; // number of cells covered 
	int count_path[2]={1, 0};  // first arrays are size 1 (the initial status), second part of the arrays is size 0 

	int col;
	while (1) {
		bufsize = fread(buffer, 1, 1, in);
		if (bufsize==0) 
		{
			break;
		}
		if (buffer[0]=='\n')
		{
			if (i > MAX_SIZE_Y) {
				fprintf(stdout, "board too high\n");
				return 1;
			}
			i+=1;
			if (p.size_x==0)
			{
				p.size_x=j;
			}
			j=0;
			p.size_y+=1;
		}
		else
		{ 
			if (j > MAX_SIZE_X) {
				fprintf(stdout, "Board too wide\n");
				return 1;
			} 
			col=(int) strtol(buffer, NULL, 10);
			if (col > 7) {
				fprintf(stdout, "Too much colors\n");
				return 1;
			} 
			if (col < min_col) { min_col=col; }
			if (col > max_col) { max_col=col; }
			board[j][i]=col;
			owneds[0][j][i]=0; // while i'm storing the boards, i'm also initializing owneds
			j+=1; 
		}
	} 
	if (!parse_parameters(argc, argv, &p)) { exit(1); }


	// we init paths with -1, will make it easier to debug
	for (i=0;i<p.max_paths_check;i++) {
		for (j=0;j<MAX_PATH;j++) {
			paths[i][j]=-1; } }

	owneds[0][p.begin_x][p.begin_y]=1;
	update_owned_2(board, owneds[0], board[p.begin_x][p.begin_y], &p);
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


		max_cov=-1;
		max_cov_index=0;
		for (j=0;j < p.max_paths_check; j++) 
		{
			if (coverts[(1-swap)*p.max_paths_check+j] > max_cov) {
				max_cov_index=(1-swap)*p.max_paths_check+j;
				max_cov=coverts[max_cov_index];
			}
		} 
		/*printf("----------------------------------------\n"); // some paths are missing
		for (i=0;i<count_path[swap];i++) { print_path(paths[i+MAX_PATHS*swap], path_length); }
		printf("----------------------------------------\n");*/

		// we'll refill from 0 the destination
		count_path[1-swap]=0;

		// for each existing path
		for (i=0;i<count_path[swap];i++) // super wrong, or is it ? well it can be. and i'm screwed. // i need a list of path to skip because removed
		{ 
			last_col=paths[swap*p.max_paths_check+i][path_length-1];
			col=min_col;
			while ((!win) && (col <= max_col))
			{ 
				if ((path_length==0) || (col!=last_col)) // either it's the first path, either we change color, otherwise we shouldn't process that color
				{
					last_cov=coverts[swap*p.max_paths_check+i];
					for (k=0;k<p.size_x;k++) {
						for (l=0;l<p.size_y;l++) {
							owned[k][l]=owneds[swap*p.max_paths_check+i][k][l];
						}
					} 
					update_owned_2(board, owned, col, &p);
					cov=get_covert(owned); 
					if (cov>last_cov) { 
						if (count_path[1-swap]==p.max_paths_check) { // number of path for the destination 
							// lets find the worst path of the destination
							min_cov=coverts[(1-swap)*p.max_paths_check];
							j=0;
							for (k=1;k<p.max_paths_check;k++) 
							{
								if (coverts[(1-swap)*p.max_paths_check+k] < min_cov) {
									min_cov=coverts[(1-swap)*p.max_paths_check+j];
									j=k;
								}
							} 
						}
						else
						{
							j=count_path[1-swap]; // index of where we'll write
							count_path[1-swap]=j+1;
						} 
						for (k=0;k<p.size_x;k++) {
							for (l=0;l<p.size_y;l++) {
								owneds[(1-swap)*p.max_paths_check+j][k][l]=owned[k][l];
							}
						} 
						for (k=0;k<path_length;k++) {
							paths[(1-swap)*p.max_paths_check+j][k]=paths[swap*p.max_paths_check+i][k]; // that seems wrong
						} 
						paths[(1-swap)*p.max_paths_check+j][path_length]=col; 
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
