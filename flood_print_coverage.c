#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "flood_lib.h"
#include "const.h"

// solve an input, and output just a sequence of datas
// will also read some command line options, to define the max size of array of solutions

int main(int argc, char *argv[])
{ 
	FILE *in = stdin; 

	int line; // number of the line currently read
	int bufsize; 
	char *buffer;
	buffer=malloc(1);
	int i=0,j=0;
	int size_x=0; // length of a line
	int size_y=0; // number of lines 
	int begin_x=0;
	int begin_y=0;
	int max_paths_check=0; // useless for print
	static int path[MAX_PATH]; // longuest path will be 100
	static int board[MAX_SIZE_X][MAX_SIZE_Y];
	static int owned[MAX_SIZE_X][MAX_SIZE_Y];
	int path_length; 

	int col, max_col;

	parse_parameters(argc, argv, &begin_x, &begin_y, &max_paths_check);

	j=0;
	int end=0;
	while (end==0) {
		bufsize = fread(buffer, 1, 1, in);
		if (bufsize==0) 
		{
			end=1;
		}
		else
		{
			if (buffer[0]=='\n')
			{
				end=1;
			}
			else
			{
				path[j]=(int) strtol(buffer, NULL, 10);
				j+=1;
			}
		}
	} 
	path_length=j;
	
	j=0;
	while (1) {
		bufsize = fread(buffer, 1, 1, in);
		if (bufsize==0) 
		{
			break;
		}
		if (buffer[0]=='\n')
		{
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
			col=(int) strtol(buffer, NULL, 10);
			if (col > max_col) { max_col=col; }
			board[j][i]=col;
			owned[j][i]=0; // while i'm storing the boards, i'm also initializing owneds
			j+=1; 
		}
	} 

	owned[begin_x][begin_y]=1;
	update_owned_2(board, owned, board[begin_x][begin_y], size_x, size_y);
	char c;
	printf("color   : ");
	for (i=0;i<path_length;i++)
	{
		col=path[i];
		printf("%4d", col);
	}

	printf("\ncoverage: ");
	for (i=0;i<path_length;i++)
	{ 
		col=path[i]; 
		update_owned_2(board, owned, col, size_x, size_y, 0);
		printf("%4d", get_covert(owned));
	}
	printf("\n");

}
