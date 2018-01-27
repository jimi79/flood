#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "flood_lib.h"
#include "const.h"


int add_parameter(char *params[], char *value, int *count) { 
	params[*count] = malloc((strlen(value) + 1) * sizeof(char));
	strcpy(params[*count], value);
	*count = *count + 1; 
}

int test_parse_parameters() {
	struct parameters *p;
	p = malloc(sizeof(struct parameters)); 
	char *context = "in parse_parameters"; 
	int count = 0;
	char *params[6];
	add_parameter(params, "blah\0", &count);
	add_parameter(params, "-bx\0", &count);
	add_parameter(params, "5\0", &count);
	add_parameter(params, "-by\0", &count);
	add_parameter(params, "25\0", &count);
	add_parameter(params, "-dn\0", &count);

	parse_parameters(count - 1, params, p);
	
	int ok = 1;
	if (p->begin_x != 5) { 
		printf("#1 parameter begin_x not read correctly, %s\n", context);
	}
	if (p->begin_y != 25) { 
		printf("#2 parameter begin_y not read correctly, %s\n", context);
	}
	if (p->display_color_number != 1) { 
		printf("#3 parameter begin_y not read correctly, %s\n", context);
	} 
	if (p->display_stars != 0) { 
		printf("#4 parameter begin_y not read correctly, %s\n", context);
	} 

	free(p);
	int i;
	for (i=0; i < count; i++) {
		free(params[i]);
	}
	return ok;
}

int test_parse_parameters_2() { 
	int ok = 1;
	struct parameters *p;
	p = malloc(sizeof(struct parameters)); 
	char *context = "in parse_parameters"; 
	int count = 0;
	char *params[2];
	add_parameter(params, "blah\0", &count);
	add_parameter(params, "-foo\0", &count); 
	printf("should complain about a -foo parameter\n");
	if (parse_parameters(count - 1, params, p)) {
		printf("didn't complain about the parameter, in %s\n", context);
		ok = 0;
	}
	free(p);
	int i;
	for (i=0; i < count; i++) {
		free(params[i]);
	}
	return ok; 
}


int test_test_parameters() {
	return 1;
}


int test_test() {
	printf("coucou\n");
}

int main(int argc, char *argv[]) {
	//test_test(); return 0;
	//printf("here\n");
	if (test_parse_parameters() &&
		  test_parse_parameters_2()
		  ) {
		printf("== ok\n");
	}
	else
	{ 
		printf("== NOT OK\n"); 
	} 
}
