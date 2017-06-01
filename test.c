#include <stdio.h>
#include <stdlib.h>

int main (int argc, const char * argv[])
{
#define SIZE 4000
	static int paths[SIZE][50][50];
	printf("here\n"); 
	float s=sizeof(paths);
	printf("size=%5.5f\n", s/1024/1024); 
}
