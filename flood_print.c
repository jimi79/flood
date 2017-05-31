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
	short int path[max_path]; // longuest path will be 100
	short int board[max_size_x][max_size_y];
	short int owned[max_size_x][max_size_y];
	int path_length; 

	int col, max_col;

	parse_parameters(argc, argv, &begin_x, &begin_y);

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

	printf("\033[2J\033[1;1H");
	owned[begin_x][begin_y]=1;
	update(board, owned, size_x, size_y, 0);
	print_board(board, owned, size_x, size_y); 
	char c;
	for (i=0;i<path_length;i++)
	{
		col=path[i];
		change_color(board, owned, col, size_x, size_y, 1);
		update(board, owned, size_x, size_y, 1);
		printf("\033[%d;%dH", size_y+2, 0);
		fflush(stdout);
		usleep(300000);
	}
	printf("\033[0m");

}
