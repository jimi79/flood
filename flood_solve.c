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
	int size_x=0; // length of a line
	int size_y=0; // number of lines 
	int min_col=9; // min col number
	int max_col=0; // max col number
	int begin_x=0;
	int begin_y=0; 
	int max_paths_check=100; // default value

	// arrays are used to store paths, boards with these paths, owneds with these paths, number of cells covered. size is 2* MAX_PATH, because first 1000 are the ccurrent paths we have, and second 1000 are the paths that we calculated from the current paths. and each turn we swap : first turn, from 0 to 999 (max, could be less) are current path, and we store calculated path from 1000 to 1999. second turn, from 1000 to 1999 are the current paths, and we store from 0 to 999, overwriting the old paths we've found that we don't care anymore

	static int paths[2*MAX_PATHS][MAX_PATH]; // longuest path will be 100
	static int boards[2*MAX_PATHS][MAX_SIZE_X][MAX_SIZE_Y];
	static int owneds[2*MAX_PATHS][MAX_SIZE_X][MAX_SIZE_Y];
	int coverts[2*MAX_PATHS]; // number of cells covered

	int count_path[2]={1, 0};  // first arrays are size 1 (the initial status), second part of the arrays is size 0

	parse_parameters(argc, argv, &begin_x, &begin_y, &max_paths_check);

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
			if (size_x==0)
			{
				size_x=j;
			}
			j=0;
			size_y+=1;
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
			boards[0][j][i]=col;
			owneds[0][j][i]=0; // while i'm storing the boards, i'm also initializing owneds
			j+=1; 
		}
	} 


	// we init paths with -1, will make it easier to debug
	for (i=0;i<max_paths_check;i++) {
		for (j=0;j<MAX_PATH;j++) {
			paths[i][j]=-1; } }

	owneds[0][begin_x][begin_y]=1;
	update(boards[0], owneds[0], size_x, size_y, 0);
	coverts[0]=get_covert(owneds[0]); 
	int win=0; 
	int index_win=0;
	int path_length=0; // length of the paths tested
	int k,l;
	int swap=0;
	int last_col;
	int cov; // coverage, cells owned
	int min_cov; // min coverage for a section. will be used to find out which line i want to remove to make room
	int max_cov;
	int max_cov_index; // index of the minimu coverage




	while (!win) {
		if (path_length > MAX_PATH) { 
			max_cov=-1;
			max_cov_index=0;
			for (i=0;i < max_paths_check; i++) 
			{
				if (coverts[(1-swap)*max_paths_check+i] > max_cov) {
					max_cov_index=(1-swap)*max_paths_check+i;
					max_cov=coverts[max_cov_index];
				}
			} 
			print_path(paths[swap*max_paths_check+max_cov_index], path_length); 
			fprintf(stderr, "path too long, %d, coverage of %d\n", path_length, max_cov); 
			return 3;
		}


		max_cov=-1;
		max_cov_index=0;
		for (j=0;j < max_paths_check; j++) 
		{
			if (coverts[(1-swap)*max_paths_check+j] > max_cov) {
				max_cov_index=(1-swap)*max_paths_check+j;
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
			last_col=paths[swap*max_paths_check+i][path_length-1];
			col=min_col;
			while ((!win) && (col <= max_col))
			{
				if ((path_length==0) || (col!=last_col)) // either it's the first path, either we change color
				{
					if (count_path[1-swap]==max_paths_check) { // number of path for the destination 
						// lets find the worst path of the destination
						min_cov=size_x*size_y+1; // cannot be matched
						for (k=0;k<max_paths_check;k++) 
						{
							if (coverts[(1-swap)*max_paths_check+k] < min_cov) {
								min_cov=coverts[(1-swap)*max_paths_check+j];
								j=k;
							}
						} 
						//printf("removing try %d, which covered %d\n", min_cov_index, min_cov);
					}
					else
					{
						j=count_path[1-swap]; // index of where we'll write
						count_path[1-swap]=j+1;
						//printf("count path = %d\n", count_path[1-swap]);
					}

					// i need to duplicate path, owned and board here !
					for (k=0;k<size_x;k++) {
						for (l=0;l<size_y;l++) {
							boards[(1-swap)*max_paths_check+j][k][l]=boards[swap*max_paths_check+i][k][l];
							owneds[(1-swap)*max_paths_check+j][k][l]=owneds[swap*max_paths_check+i][k][l];
						}
					} 

					for (k=0;k<path_length;k++) {
						paths[(1-swap)*max_paths_check+j][k]=paths[swap*max_paths_check+i][k]; // that seems wrong
					}

					coverts[(1-swap)*max_paths_check+j]=coverts[swap*max_paths_check+i]; // useless, will be recalculated 
					paths[(1-swap)*max_paths_check+j][path_length]=col; 

					// first, all cells owned turn to the new color
					change_color(boards[(1-swap)*max_paths_check+j], owneds[(1-swap)*max_paths_check+j], col, size_x, size_y, 0);
					update(boards[(1-swap)*max_paths_check+j], owneds[(1-swap)*max_paths_check+j], size_x, size_y, 0);
						
					// check if it's a win
					cov=get_covert(owneds[(1-swap)*max_paths_check+j]);
					coverts[(1-swap)*max_paths_check+j]=cov;
					if (cov==size_x*size_y) { win=1; index_win=(1-swap)*max_paths_check+j; }; 
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
