#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "const.h"

int get_covert(int owned[MAX_SIZE_X][MAX_SIZE_Y])
{
	int cpt=0;
	int i,j;
	for (i=0;i<MAX_SIZE_X;i++)
	{
		for (j=0;j<MAX_SIZE_Y;j++)
		{
			cpt+=owned[i][j]==1;
		}
	}
	return cpt;
}

int parse_parameters(int argc, char *argv[], int *begin_x, int *begin_y, int *max_path)
{
	int i; 
	for(i = 1; i < argc; i++) {
		char *ptr = strchr(argv[i], '=');
		if (ptr) {
			int index = ptr - argv[i];
			char code[index];
			memcpy(code, &argv[i][0], index);
			code[index] = '\0'; 
			char value[strlen(argv[i]) - index];
			memcpy(value, &argv[i][index + 1], strlen(argv[i]) - index);
			int ok=0;
			if (!strcmp(code, "bx")) { 
				*begin_x=(int) strtol(value, NULL, 10);
				ok=1;
			}
			if (!strcmp(code, "by")) {
				*begin_y=(int) strtol(value, NULL, 10);
				ok=1;
			} 
			if (!strcmp(code, "mp")) {
				*max_path=(int) strtol(value, NULL, 10);
				ok=1;
			} 
		} else
		{ fprintf(stderr, "Error, all parameters should be code=value. '%s' isn't\nUsage is flood_xxx bx=start_x_position by=start_y_position mp=max_paths_checked", argv[i]); return 1; };
	};
	return 0;
}; 

int colorize(int x, int y, int col) 
{
	//printf("painting %d %d in %d\n", x, y, col);
	//return 0;
	int bg,fg;
	bg=col; 
	if ((bg==2)||(bg==3)||(bg==6)||(bg==7)) 
	{
		fg=0;
	}
	else
	{ 
		fg=7; 
	} 
	printf("\033[%dm\033[%dm", bg+40, fg+30); // we change colors
	printf("\033[%d;%dH", y+1, x*2+1); // we move the cursor
	printf("**"); 
	return 0;
}

int print_board(int board[MAX_SIZE_X][MAX_SIZE_Y], int owned[MAX_SIZE_X][MAX_SIZE_Y], int size_x, int size_y)
{
	int i, j;
	int bg, fg;
	FILE *out=stdout;
	for (i=0;i<size_y;i++)
	{
		for (j=0;j<size_x;j++)
		{
			bg=board[j][i];
			fprintf(out,"\033[%dm", bg+40);
			if (owned[j][i]==1) {
				if ((bg==2)||(bg==3)||(bg==6)||(bg==7)) 
				{
					fg=0;
				}
				else
				{ 
					fg=7; 
				}
				fprintf(out,"\033[%dm**",fg+30);
			}
			else
			{
				fprintf(out,"  ");
			}
			fprintf(out,"\033[0m");
		}
		fprintf(out,"\n");
	} 
	return 0;
}

int update(int board[MAX_SIZE_X][MAX_SIZE_Y], int owned[MAX_SIZE_X][MAX_SIZE_Y], int size_x, int size_y, int live_print)
{
	// for each pass, we count what we update
	// at each pass, we look if it's owned. then for 8 directions, we look if not owned, and same color. if so, direction is owned
	int updated=1; // wrong value just to start the loop
	int dir; // direction counter, we got 4 directions to check
	int i,j; // to go through the array
	int ni,nj; // new values to compare with
	int shifti, shiftj; // value to shift to check the neighbour
	while (updated > 0) 
	{ 
		updated=0;
		for (i=0; i<size_x;i++)
		{
			for (j=0; j<size_y;j++)
			{
				// we check only if we own the first cell
				if (owned[i][j]==1)
				{
					if (DEBUG) { printf("checking around cell %d,%d\n", i, j); }
					for (dir=0;dir<4;dir++)
					{ 
						if (dir==0) { shifti=-1;shiftj=0; }
						if (dir==1) { shifti=1;shiftj=0; }
						if (dir==2) { shifti=0;shiftj=-1; }
						if (dir==3) { shifti=0;shiftj=1; } 
						if (DEBUG) { printf("shifts are %d,%d\n", shifti, shiftj); }
						ni=i+shifti; nj=j+shiftj;
						if ((ni>=0) && (nj>=0) && (ni<size_x) && (nj<size_y))
						{
							if (DEBUG) { printf("checking with cell %d,%d\n", ni, nj); }
							if (owned[ni][nj]==0) 
							{
								if (board[i][j]==board[ni][nj])
								{
									if (DEBUG) { printf("updating cell %d,%d\n", ni, nj); }
									if (live_print) {
										//printf("update:");
										colorize(ni, nj, board[i][j]);
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

int change_color(int board[MAX_SIZE_X][MAX_SIZE_Y], int owned[MAX_SIZE_X][MAX_SIZE_Y], int col, int size_x, int size_y, int live_print)
{
	int i,j;
	for (i=0;i<size_x;i++) {
		for (j=0;j<size_y;j++) { 
			if (owned[i][j]) {
				board[i][j]=col; 
				if (live_print)
				{
					//printf("change_color:");
					colorize(i, j, col);
				}
			}
		} 
	} 
	return 0;
} 
