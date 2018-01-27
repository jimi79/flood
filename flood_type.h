// define structure here to store size_x, size_y, display_color_number (dn), display_star (ds), max_path, begin_x, begin_y, and display_stats (dst)
#include "const.h"

struct parameters
{
	char path[MAX_PATH];
	int begin_x, begin_y;
	int display_color_number;
	int display_stars;
	int display_stats; // to display the color and percentage coverage along
	int max_paths_check;
	int min_col, max_col;
	int size_x, size_y;
	int surface;
	int max_cpu;
};

