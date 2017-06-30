#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <time.h>

int main(int argc, char *argv[])
{
	if (argc < 3) {
		printf("Usage : size_x size_y color_count\n");
		return 1;
	}
	int size_x = (int) strtol(argv[1], NULL, 10);
	int size_y = (int) strtol(argv[2], NULL, 10);
	int color_count = (int) strtol(argv[3], NULL, 10);

	srand(time(NULL));
	int i,j;
	int r;
	for (i=0; i<size_y; i++) 
	{
		for (j=0; j<size_x; j++)
		{
			r=rand() % (color_count); // wrong, will not evenly distribute datas
			printf("%d", r);
		}
		printf("\n");
	}

	return 0;
};

