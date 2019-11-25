#define CRT_SECURE_NO_WARNINGS
#define HAVE_STRUCT_TIMESPEC


#include <stdlib.h>
#include <ctype.h>
#include <semaphore.h>
#include <pthread.h>

#include "Fore/turbine_defines.h"
#include "Fore/time_functions_19.h"
#include "Fore/setpath_defs.h"

#ifndef FILE
	#include <string.h>
	#include <stdio.h>
#endif



#include "init/variables.h"
#include "init/assign_file.h"
#include "init/define_grid_neighbors.h"
#include "init/init_sem.h"
#include "init/init_threads.h"

#include "thread/collecting_sums.h"
//#include "thread/calc_sum_average.h"
#include "thread/calc_sum_library/v1.h"
#include "thread/update_turbine_power.h"
#include "thread/sleep_until_broadcast.h"
	#include "thread/thread_behavior.h"
		#include "thread/thread.h"

#include "output/new_average.h"
	#include "output/print_matrix.h"
		#include "output/read_results.h"

//MAIN-----------------------
int main(){	
	setpath311('3');
	assign_file(current_pow, max_pow, grid, &target_vals, "/home/student/Desktop/Project3/input-output/turbine_test_input.txt");
	outfile = fopen(out_path, "a");
	print_matrix(outfile);
	define_grid_neighbors(COLCOUNT, ROWCOUNT, neighbors, num_neighbors);
	init_sem(thread_sem);
	init_threads(tid, thread, grid);
	read_results(&target_vals, outfile);	
	stop_timing();
	printf("wall time: %f, cpu time: %i\n", get_wall_clock_diff(), get_nanodiff());
	return 0;
}

//in_path

