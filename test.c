#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int a[5][5];
	int i,j;
	for (i=0;i<5;i++) {
		for (j=0;j<5;j++) {
			a[i][j]=i*j;
		}
	}
	printf("%d\n", a[2][3]);
}

