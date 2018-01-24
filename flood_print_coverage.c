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

	struct parameters p;
	p.size_x=0; p.size_y=0; p.begin_x=0; p.begin_y=0; 
	p.max_paths_check=0;

	static int path[MAX_PATH]; // longuest path will be 100
	static int board[MAX_SIZE_X][MAX_SIZE_Y];
	static int owned[MAX_SIZE_X][MAX_SIZE_Y];
	int path_length; 
	int col, max_col;
	if (!parse_parameters(argc, argv, &p)) { exit(1); }

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
			if (p.size_x==0)
			{
				p.size_x=j;
			}
			j=0;
			p.size_y+=1;
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

	owned[p.begin_x][p.begin_y]=1;
	update_owned_2(board, owned, board[p.begin_x][p.begin_y], &p);


	int a_cov[path_length];
	double a_per[path_length];
	double a_diff[path_length];
	double a_diff_per[path_length];

	int cov;
	int tot=p.size_x * p.size_y;
	for (i=0;i<path_length;i++)
	{ 
		col=path[i]; 
		update_owned_2(board, owned, col, &p);
		cov = get_covert(owned);
		a_cov[i] = cov;
		a_per[i] = cov * 100.0 / tot;
		if (i==0) {
			a_diff[i]=0;
			a_diff_per[i]=0;
		}
		else {
			a_diff[i] = a_cov[i] - a_cov[i-1];
			a_diff_per[i] = a_diff[i] * 100.0 / tot; 
		}
	}


	printf(  "color     : ");
	for (i=0;i<path_length;i++)
	{
		col=path[i];
		printf("%5d", col);
	} 
	printf("\ncoverage  : ");
	for (i=0;i<path_length;i++)
	{ 
		col=path[i]; 
		printf("%5d", a_cov[i]);
	}

	printf("\npercentage: ");
	for (i=0;i<path_length;i++)
	{ 
		col=path[i]; 
		printf("%5.0f", a_per[i]);
	}

	printf("\ndifference: ");
	for (i=0;i<path_length;i++)
	{ 
		col=path[i]; 
		printf("%5.0f", a_diff[i]);
	}

	printf("\ndiff %%    : ");
	for (i=0;i<path_length;i++)
	{ 
		col=path[i]; 
		printf("%5.0f", a_diff_per[i]);
	}
		
	
	printf("\n");

}
