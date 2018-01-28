#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "const.h"
#include "flood_type.h"

int get_covert(int owned[MAX_SIZE_X][MAX_SIZE_Y]) {
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

int init_parameters(struct parameters *p) { 
	p->min_col = 0;
	p->max_col = 9;
	p->begin_x = 0;
	p->begin_y = 0;
	p->max_paths_check = 100;
	p->display_color_number = 0;
	p->display_stars = 0;
	p->display_stats = 0; 
	p->surface = 0;
	p->max_cpu = 4;
}


int parse_parameters(int argc, char *argv[], struct parameters *p, int path[MAX_PATHS]) { // use ->truc instead of &val.truc 
	init_parameters(p); 
	int i; 
	int ok = 1;
	i = 0;
	i = 1;
	while ((i<argc) && (ok = 1)) {
		ok=0;
		if (!strcmp(&argv[i][0], "-bx")) {
			i++;
			p->begin_x = (int) strtol(&argv[i][0], NULL, 10);
			ok=1;
		}	
		if (!strcmp(&argv[i][0], "-by")) {
			i++;
			p->begin_y = (int) strtol(&argv[i][0], NULL, 10);
			ok=1;
		}	
		if (!strcmp(&argv[i][0], "-mp")) {
			i++;
			p->max_paths_check = (int) strtol(&argv[i][0], NULL, 10);
			ok=1;
		}	
		if (!strcmp(&argv[i][0], "-dn")) {
			p->display_color_number = 1;
			ok=1;
		}	
		if (!strcmp(&argv[i][0], "-ds")) {
			p->display_stars = 1;
			ok=1;
		}	
		if (!strcmp(&argv[i][0], "-dst")) {
			p->display_stats = 1;
			ok=1;
		}	
		if (!strcmp(&argv[i][0], "-path")) {
			i++;
			strcpy(p->path, &argv[i][0]);
			ok=1;
		}
		if (!strcmp(&argv[i][0], "-cpu")) {
			i++;
			p->max_cpu = (int) strtol(&argv[i][0], NULL, 10);
			ok=1;
		} 
		if (ok == 0) {
			printf("Unknown parameter %s\n", &argv[i][0]); 
		}
		i++;
	}; 

	return ok;
}; 

int test_parameters(struct parameters *p) {
	int ok = 1;
	if (p->begin_x > p->size_x) { fprintf(stderr, "Error: horizontal starting position is greater than horizontal size\n"); ok=0; };
	if (p->begin_y > p->size_y) { fprintf(stderr, "Error: vertical starting position is greater than vertical size\n"); ok=0; };
	if (p->size_x > MAX_SIZE_X) { fprintf(stderr, "Error: horizontal size greater than %d\n", MAX_SIZE_X); ok=0; };
	if (p->size_y > MAX_SIZE_Y) { fprintf(stderr, "Error: vertical size greater than %d\n", MAX_SIZE_Y); ok=0; };
	if (p->max_paths_check > MAX_PATHS) { fprintf(stderr, "Error: maximum number of paths checks is size greater than %d\n", MAX_PATHS); ok=0; };

	//TODO : some programs can't use some parameters, check that based on the program run

	return ok;
} 

int color_print(int x, int y, int col, struct parameters *p) {
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
	if (p->display_stars) { printf("**"); } else { printf("  "); }
	return 0;
}

int update_owned(int board[MAX_SIZE_X][MAX_SIZE_Y], int owned[MAX_SIZE_X][MAX_SIZE_Y], int color, struct parameters *p) {
// doesn't update the color of the board
	int i, j, updated;
	updated=1;
	while (updated) {
		updated=0;
		for(i=0;i<p->size_x;i++) {
			for(j=0;j<p->size_y;j++) {
				if (owned[i][j]) {
					if (i>0) {
						if (!(owned[i-1][j])) {
							if (board[i-1][j]==color) { owned[i-1][j]=1; updated=1; }
						}
					}
					if (j>0) {
						if (!(owned[i][j-1])) {
							if (board[i][j-1]==color) { owned[i][j-1]=1; updated=1; }
						}
					}
					if (i<p->size_x-1) {
						if (!(owned[i+1][j])) {
							if (board[i+1][j]==color) { owned[i+1][j]=1; }
						}
					}
					if (j<p->size_y-1) {
						if (!(owned[i][j+1])) {
							if (board[i][j+1]==color) { owned[i][j+1]=1; }
						}
					}
				}
			}
		}
	} 
}

int init_board_from_stdin(int board[MAX_SIZE_X][MAX_SIZE_Y], struct parameters *p) {
	FILE *in = stdin; 
	int bufsize; 
	char *buffer;
	int i=0,j=0, col;
	buffer=malloc(1);
	p->size_x = 0; p->size_y = 0;
	p->min_col = 9; p->max_col = 0;
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
			if (p->size_x==0)
			{
				p->size_x=j;
			}
			j=0;
			p->size_y+=1;
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
			if (col < p->min_col) { p->min_col=col; }
			if (col > p->max_col) { p->max_col=col; }
			board[j][i]=col;
			j+=1; 
		}
	} 
	p->surface = p->size_x * p->size_y;
	return 1;
}

int init_owned(int owned[MAX_SIZE_X][MAX_SIZE_Y], struct parameters *p) {
	int i, j;
	for (i=0; i < p->size_x; i++) {
		for (j=0; i < p->size_y; i++) {
			owned[i][j] = 0;
		}
	} 
	owned[p->begin_x][p->begin_y]=1;
}

int print_board(int board[MAX_SIZE_X][MAX_SIZE_Y], int owned[MAX_SIZE_X][MAX_SIZE_Y], struct parameters *p) {
	int i, j;
	int bg, fg;
	FILE *out=stdout;
	for (i=0;i<p->size_y;i++)
	{
	  for (j=0;j<p->size_x;j++)
	  {
	    bg=board[j][i];
	    if ((bg==2)||(bg==3)||(bg==6)||(bg==7))
	    {
	      fg=0;
	    }
	    else
	    {
	      fg=7;
	    }
	    fprintf(out,"\033[%dm\033[%dm", fg+30, bg+40);
	    if (owned[j][i]==1) {
	      if (p->display_stars) {
	        fprintf(out,"**");
	      }
	      else
	      {
	        fprintf(out,"  ");
	      }
	    }
	    else
	    {
	      if (p->display_color_number) {
	        fprintf(out," %d", bg); // has to be an option
	      }
	      else
	      {
	        fprintf(out, "  ");
	      }
	    }
	    fprintf(out,"\033[0m");
	  }
	  fprintf(out,"\n");
	}
	return 0;
}

int print_path(int path[MAX_PATH], int path_length) // only for debug
{
	int i;
	for (i=0;i<path_length;i++)
	{
		if (path[i]!=-1) { printf("%d", path[i]); }
	}
	//printf("\n");
	return 0;
}


