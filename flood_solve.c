#include "const.h"
#include "flood_lib.h"
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct parameters p;
static int paths[2*MAX_PATHS][MAX_PATH]; // longuest path will be 100///copy array by bytes
static int board[MAX_SIZE_X][MAX_SIZE_Y];
static int owneds[2*MAX_PATHS][MAX_SIZE_X][MAX_SIZE_Y];
int coverts[2*MAX_PATHS]; // number of cells covered 
int count_path[2]={1, 0};  // first arrays are size 1 (the initial status), second part of the arrays is size 0 
int win=0; 
int index_win=0;
int path_length=0; // length of the paths tested
int swap=0;
int max_cov;
int max_cov_index; // index of the minimu coverage 
int index_in, index_out; // index of the input, before we try a new color, and index of the output with each possible color 

struct t_th_par {
	int index_path; // path to evaluate for each color
};

pthread_mutex_t mutex_write_res = PTHREAD_MUTEX_INITIALIZER;

void *thread_test_path(void *args) {
	int i=0;
	struct t_th_par *th_par = (struct t_th_par *) args;
	int index_path = th_par->index_path; 

	if (DEBUG) { printf("evaluating path %d\n", index_path); }

	int last_color=99; // 99 means it's the first color, the path are empty
	int color;
	int j,k; // temp values
	int owned[MAX_SIZE_X][MAX_SIZE_Y]; // temp 'owned' to check if some path is worth being stored
	int last_cov;
	int cov;
	int min_cov; // min coverage for a section. will be used to find out which line i want to remove to make room
	if (path_length>0) {
		last_color=paths[index_in+index_path][path_length-1];
	}
	color=p.min_col;
	while ((!win) && (color <= p.max_col))
	{
		// if it's the first path, or we didn't change color, then we don't do anythg. otherwise we try that new color
		if ((path_length==0) || (color!=last_color)) 
		{
			last_cov=coverts[index_in+index_path]; 
			// we copy the array into a temporary owned that will be updated and evaulated 
			memcpy(owned[0], owneds[index_in+index_path][0], MAX_SURFACE*sizeof(int));  
			if (DEBUG) { printf("evaluating path %d, color %d\n", index_path, color); }
			update_owned(board, owned, color, &p);
			cov=get_covert(owned); 
			// if that new color added to the path helped
			if (cov>last_cov) { 
				// if we reached the limit
				pthread_mutex_lock(&mutex_write_res);
				if (count_path[1-swap]==p.max_paths_check) { // number of path for the destination 
					// lets find the worst path of the destination
					min_cov=coverts[index_out]; 
					j=0;
					for (k=1;k<p.max_paths_check;k++) 
					{
						if (coverts[index_out+k] < min_cov) {
							min_cov=coverts[index_out+k];
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
				memcpy(owneds[index_out+j], owned[0], MAX_SURFACE*sizeof(int));
				// we write the new path
				for (k=0;k<path_length;k++) {
					paths[index_out+j][k]=paths[index_in+index_path][k]; // that seems wrong
				} 
				paths[index_out+j][path_length]=color; 
				// we write the new covert
				coverts[index_out+j]=cov;
				// check if it's a win
				if (DEBUG) { printf("cover %d for path=", cov); 
					print_path(paths[index_out+j], path_length+1);
				} 
				pthread_mutex_unlock(&mutex_write_res);
				if (cov==p.size_x*p.size_y) { win=1; index_win=(1-swap)*p.max_paths_check+j; }; 
			} 
		}
		color++;
	}
}



int main(int argc, char *argv[])
{ 
	//signal(SIGSEGV, handler);   // install our handler -> backtrace sucks

	if (!parse_parameters(argc, argv, &p)) { exit(1); } 

	struct t_th_par th_par[p.max_cpu];
	pthread_t tid[p.max_cpu];

	// we init paths with -1, will make it easier to DEBUG
	int i, j;
	for (i=0;i<p.max_paths_check;i++) {
		for (j=0;j<MAX_PATH;j++) {
			paths[i][j]=-1; } } 
	init_board_from_stdin(board, &p); 
	if (DEBUG) { printf("max col = %d\n", p.max_col); }

	if (!test_parameters(&p)) { exit(1); } // check if parameters match the actual board size
	init_owned(owneds[0], &p);
	update_owned(board, owneds[0], board[p.begin_x][p.begin_y], &p);
	coverts[0]=get_covert(owneds[0]); 
	while (!win) {
		index_in=(swap)*p.max_paths_check;
		index_out=(1-swap)*p.max_paths_check; 

		if (path_length > MAX_PATH) { 
			max_cov=-1;
			max_cov_index=0;
			for (i=0;i < p.max_paths_check; i++) 
			{
				if (coverts[index_in+i] > max_cov) {
					max_cov_index=index_in+i;
					max_cov=coverts[max_cov_index];
				}
			} 
			print_path(paths[index_in+max_cov_index], path_length); 
			fprintf(stderr, "path too long, %d, coverage of %d\n", path_length, max_cov); 
			return 3;
		}

		// we write the output array (other part) as empty
		count_path[1-swap]=0;
		int count; // number of threads to start
		count = count_path[swap]; 
		i = 0;
		while (i < count) {
			int j = 0;
			int allocated = 0;

			while ((j < p.max_cpu) && (i < count)) { 
				allocated++;
				th_par[j].index_path=i;
				pthread_create(&tid[j], NULL, thread_test_path, &th_par[j]);
				j++;
				i++;
			}
			for (j = 0; j < allocated; j++)
			{
				pthread_join(tid[j], NULL);
			} 
		}
		path_length+=1;
		if (DEBUG) { printf("path lengh = %d\n", path_length); }
		swap=1-swap; // we swap here
	}
	print_path(paths[index_win], path_length);
	printf("\n");
	return 0;
} 
