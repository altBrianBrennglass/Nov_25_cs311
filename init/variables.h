//GLOBAL VARIABLES-------------------------------------------------------------------------------------
	/*CONTROL VARIABLES FOR THREADS
		thread_mux | used for preventing the a neighbor from changing its value until all its neighbors have accessed its values
		thread_cond_var | How a turbine checks if all its neighbors have accessed it
		thread_sem | allows only 1 thread at a time to access another threads value and change the condition variable that prevents
				   the other thread from incrementing
		safe_to_update_grid | the actual condition variable that is checked to see if a thread can modify its value
		wait_for_broadcast | Threads will sleep until main prints their values. This forces them asleep
	*/
	pthread_mutex_t thread_mux[COLCOUNT][ROWCOUNT];
	pthread_cond_t thread_cond_var[COLCOUNT][ROWCOUNT];
	sem_t thread_sem[COLCOUNT][ROWCOUNT];
	int safe_to_update_grid[COLCOUNT][ROWCOUNT] = {{0}};
	pthread_mutex_t wait_for_broadcast = PTHREAD_MUTEX_INITIALIZER;

	/*CONTROL VARIABLES FOR MAIN
		controller | mutex for preventing main from printing new values to console before everything has updated
		controller_var | controller condition variable
		ready_to_print | controller condition variable counter / decrements every time thread finished updating its value
		received_broadcast | allows main to wake up all the threads
	*/
	pthread_mutex_t controller = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t controller_var = PTHREAD_COND_INITIALIZER;
	pthread_cond_t received_broadcast = PTHREAD_COND_INITIALIZER;
	int ready_to_print = COLCOUNT * ROWCOUNT;

	/*GRID TRACKING VARIABLES
		current_pow | grid of current powers
		max_pow | grid of max_pow
		nieghbors | grid of nieghbors
		num_neighbors | 4d grid of neighbors
		sum | a grid that is used to temporarily used to store the new value of a turbine
		target_vals | a struct that holds the next power request and two counters that determine how many cycles are left
		grid_position | a struct that allows a turbine to know where it is
	*/
		float current_pow[COLCOUNT][ROWCOUNT];
		float max_pow[COLCOUNT][ROWCOUNT];
		int neighbors[COLCOUNT][ROWCOUNT][4][2];
		int num_neighbors[COLCOUNT][ROWCOUNT] = {{0}};
		float sum[COLCOUNT][ROWCOUNT] = {{0}};
		float average = 0;
		
		struct target_vals{
			int current_target;
			int end_target;
			float values[200];
		};
		struct target_vals target_vals = {.current_target = 0, .end_target = 0};

		struct grid_position {
			int col; //COL
			int row; //ROW
		};
		struct grid_position grid[COLCOUNT][ROWCOUNT];
		
	/*THREAD VARIABLES
		tid | thread IDs
	*/
		pthread_t tid[COLCOUNT][ROWCOUNT];


int i = 0;
//----------------------------------------------------------------------------------------------------------------



