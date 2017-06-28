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

int parse_parameters(int argc, char *argv[], struct params *p) { // use ->truc instead of &val.truc
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
				p->begin_x=(int) strtol(value, NULL, 10);
				ok=1;
			}
			if (!strcmp(code, "by")) {
				p->begin_y=(int) strtol(value, NULL, 10);
				ok=1;
			} 
			if (!strcmp(code, "mp")) {
				p->max_paths_check=(int) strtol(value, NULL, 10);
				ok=1;
			} 
			if (!strcmp(code, "dn")) { 
				p->display_color_number=(int) strtol(value, NULL, 10); // 1 for yes, 0 for no
				ok=1;
			} 
			if (!strcmp(code, "ds")) { 
				p->display_star=(int) strtol(value, NULL, 10); // 1 for yes, 0 for no
				ok=1;
			} 
			if (!strcmp(code, "dst")) { 
				p->display_stat=(int) strtol(value, NULL, 10); // 1 for yes, 0 for no
				ok=1;
			} 
			
		} else
		{ fprintf(stderr, "Error, all parameters should be code=value. '%s' isn't\nUsage is flood_xxx bx=start_x_position by=start_y_position mp=max_paths_checked", argv[i]); return 1; };
	};
	return 0;
}; 

int color_print(int x, int y, int col, struct params *p) {
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
	if (p->display_star) { printf("**"); } else { printf("  "); }
	return 0;
}

int print_board(int board[MAX_SIZE_X][MAX_SIZE_Y], int owned[MAX_SIZE_X][MAX_SIZE_Y], struct params *p) {
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
				if (p->display_star) {
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

int update_owned_2(int board[MAX_SIZE_X][MAX_SIZE_Y], int owned[MAX_SIZE_X][MAX_SIZE_Y], int color, struct params *p) {
	// loop each cell
	// for each, check around, and do a sum multiply some formula, to find out which cell is not owned around quickly.....
	// just a list of tests
	
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

