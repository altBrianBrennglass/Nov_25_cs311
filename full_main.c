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



//----------------------------------------------------------------------------------------------------------------





/*------------------------------------------------------------------
Reads file and assigns values to the turbines
--------------------------------------------------------------------*/
void assign_file(float current_pow[COLCOUNT][ROWCOUNT], float max_pow[COLCOUNT][ROWCOUNT], struct grid_position grid_position[COLCOUNT][ROWCOUNT], struct target_vals *targets, char *path){	
	//FILE *fp;
	infile = fopen(path, "r");
	if( infile == NULL){
		printf("could not open file\n");
		exit(EXIT_FAILURE);
	}

	char buf[50];
	const int MAX = 49;
	char *token;
	int col;
	int row;
	int stage = 0;

	while(fgets(buf, MAX, infile) != NULL){	
		char temp_buf[49];
		strcpy(temp_buf, buf);
		token = strtok(temp_buf, " \n");
		while(token != NULL){
			if(token[0] == '\"'){
				stage++;
				col = 0;
				row = 0;
			}
			switch (stage){
				case 1://insert max power into grid
					if(isdigit(token[0])){
						max_pow[col][row] = atof(token);
						grid_position[col][row].col = col;
						grid_position[col][row].row = row;
						++col;
						col %= COLCOUNT;
						!col? row++: 0;
						
					}
					break;
				case 2://insert initial power into grid
					if(isdigit(token[0])){
						current_pow[col][row] = atof(token);
						average += current_pow[col][row];
						++col;
						col %= COLCOUNT;
						!col? row++: 0;
					}
					break;
				case 3://store target values for later use
					if(isdigit(token[0])){
						targets->values[targets->end_target] = atof(token);
						targets->end_target++ ;
					}
					break;
			}
			token = strtok(NULL, " \n");
		}
	}
	fclose(infile);	
	average /= ROWCOUNT *COLCOUNT;

	start_timing();
} 




/*------------------------------------------------------------------
Finds out a turbine's neighbors, and stores the results into a 2d array
--------------------------------------------------------------------*/
void define_grid_neighbors(int col, int row, int neighbors[COLCOUNT][ROWCOUNT][4][2], int num_neighbors[COLCOUNT][ROWCOUNT]){
        int max_col = --col;
        int max_row = --row;

        for(;col >= 0;col--){
                for(row = max_row; row >= 0; row--){
		          //Northern neighbor
		          neighbors[col][row][0][0] = row == 0 ? -1: (num_neighbors[col][row]++, col);
		          neighbors[col][row][0][1] = row - 1;

		          //Right neighbor
		          neighbors[col][row][1][0] = col + 1 > max_col ? -1 : (num_neighbors[col][row]++, col + 1);
		          neighbors[col][row][1][1] = row;

		          //Southern neighbor
		          neighbors[col][row][2][0] = row + 1 > max_row ? -1 : (num_neighbors[col][row]++, col);
		          neighbors[col][row][2][1] = row + 1;

		          //Left neighbor
		          neighbors[col][row][3][0] = col == 0 ? -1 : (num_neighbors[col][row]++, col - 1);
		          neighbors[col][row][3][1] = row;
			}
			
		}
}



/*------------------------------------------------------------------
initializes sempahors
--------------------------------------------------------------------*/
void init_sem(sem_t sem[COLCOUNT][ROWCOUNT]){
	int col;
	int row;
	for(row = 0; row < ROWCOUNT; row++){
		for(col = 0; col < COLCOUNT; col++){
			sem_init(&sem[col][row], 0, 1);
		}
	}
}



/*------------------------------------------------------------------
initializes the threads
--------------------------------------------------------------------*/
void init_threads(pthread_t tid[COLCOUNT][ROWCOUNT], void * (*thread_behavior)(void *), struct grid_position grid[COLCOUNT][ROWCOUNT]){
	int col;
	int row;
	for(row = 0; row < ROWCOUNT; row++){
		for(col = 0; col < COLCOUNT; col++){
			pthread_create(&tid[col][row], NULL, thread_behavior, &grid[col][row]);
		}
	}
}




void collecting_sums(struct grid_position *turbine){ 
  int i;
   for(i = 0; i < 4; i++){
                if(!(neighbors[turbine->col][turbine->row][i][0] < 0)){//if the relevent neighbor exists
				  //add the numbers power to the sum
		            sum[turbine->col][turbine->row] += current_pow[neighbors[turbine->col][turbine->row][i][0]]
		                                              		   [neighbors[turbine->col][turbine->row][i][1]];

				  //Increment a condition variable that will allow the neighbor to change its value. Prevents other threads from 					    changing its value at the same time.
				  sem_wait(&thread_sem[neighbors[turbine->col][turbine->row][i][0]]
						            [neighbors[turbine->col][turbine->row][i][1]]);
						
						safe_to_update_grid[neighbors[turbine->col][turbine->row][i][0]]
						                   [neighbors[turbine->col][turbine->row][i][1]]++;

						pthread_cond_signal(&thread_cond_var[neighbors[turbine->col][turbine->row][i][0]]
						                                    [neighbors[turbine->col][turbine->row][i][1]]);

				  //Unlocking this will cause an error. There needs to be another locking mechanism independent of thread_mux
				  sem_post(&thread_sem[neighbors[turbine->col][turbine->row][i][0]]
						            [neighbors[turbine->col][turbine->row][i][1]]);
			}
	}
}



	 //store new power into sum variable after performing a series of equations
void calc_sum_average(struct grid_position *turbine){
	 sum[turbine->col][turbine->row] += current_pow[turbine->col][turbine->row];
	 sum[turbine->col][turbine->row] /= num_neighbors[turbine->col][turbine->row] + 1;
	 if(target_vals.values[target_vals.current_target] < average){
	 	if(sum[turbine->col][turbine->row] < target_vals.values[target_vals.current_target]){
			    //printf("sum <  | %f [%i][%i]\n", sum[turbine->col][turbine->row], turbine->col, turbine->row);
			    sum[turbine->col][turbine->row] + .3 * sum[turbine->col][turbine->row] > max_pow[turbine->col][turbine->row]? 
											   sum[turbine->col][turbine->row] = max_pow[turbine->col][turbine->row]:
			                                       (sum[turbine->col][turbine->row] += .3 * sum[turbine->col][turbine->row]);
		}
	     else{
		 	if(sum[turbine->col][turbine->row] > target_vals.values[target_vals.current_target]){
					  //printf("sum >  | %f [%i][%i]\n", sum[turbine->col][turbine->row], turbine->col, turbine->row);
				       sum[turbine->col][turbine->row] - .3 * sum[turbine->col][turbine->row] < 0?
													 sum[turbine->col][turbine->row] = 0:
					  (sum[turbine->col][turbine->row] -= .3 * sum[turbine->col][turbine->row]);
			}
		}
	}
}
//#include "thread/calc_sum_library/v1.h"


void update_turbine_power(struct grid_position *turbine){
	pthread_mutex_lock(&thread_mux[turbine->col][turbine->row]);//modify current power
		//Wait for condition
		 while(safe_to_update_grid[turbine->col][turbine->row] < num_neighbors[turbine->col][turbine->row])
			    pthread_cond_wait(&thread_cond_var[turbine->col][turbine->row], &thread_mux[turbine->col][turbine->row]);

		 //assign current power
		 current_pow[turbine->col][turbine->row] = sum[turbine->col][turbine->row];

		 //reset condition variable for next cycle
		 safe_to_update_grid[turbine->col][turbine->row] = 0;
		 ready_to_print--;
		 pthread_cond_signal(&controller_var);
	pthread_mutex_unlock(&thread_mux[turbine->col][turbine->row]);
}


void sleep_until_broadcast(){
	pthread_mutex_lock(&wait_for_broadcast);
		pthread_cond_wait(&received_broadcast, &wait_for_broadcast);
	pthread_mutex_unlock(&wait_for_broadcast);
}


/*------------------------------------------------------------------
Defines the thread behavior | Find's neighbor turbine values, performs a calculation on them and then changes its values when it is ready
--------------------------------------------------------------------*/
void *thread_behavior(void* grid){

   struct grid_position *turbine = (struct grid_position *) grid;
	//printf("%i, %i\n", turbine[0][0].col, turbine[0][0].row);
   

   collecting_sums(turbine);
   calc_sum_average(turbine);
   update_turbine_power(turbine);
   sleep_until_broadcast();

	return NULL;
}




/*------------------------------------------------------------------
Ensures a thread is active until the until the target's were read.
--------------------------------------------------------------------*/
void *thread(void *position){
	//struct grid_position *grid = (struct grid_position *) position;
	while(target_vals.current_target != target_vals.end_target){
		thread_behavior(position);
	}
	return NULL;
}



float new_average(){
	int col;
	int row;
	average = 0;
	if(target_vals.current_target)
		for(col = 0; col < COLCOUNT; col++){
			for (row = 0; row < ROWCOUNT; row++){
				average += sum[col][row];
			}
		}
	else
		for(col = 0; col < COLCOUNT; col++){
				for (row = 0; row < ROWCOUNT; row++){
					average += current_pow[col][row];
				}
			}
	return average /= ROWCOUNT*COLCOUNT;
}


/*------------------------------------------------------------------
Prints the matrix
--------------------------------------------------------------------*/
void print_matrix(FILE *ptr){
	int col;
	int row;
	//fprintf(ptr, "current values:\n");

	printf("\ncurrent_target = %i\n\n", target_vals.current_target);
	printf("target_value = %f\n\n", target_vals.values[target_vals.current_target]);
	printf("current values:\n\n");

	for(row = 0; row < ROWCOUNT; row++){
		for(col = 0; col < COLCOUNT; col++){
			//fprintf(ptr, "turbine[%i][%i] %f| ", col, row, current_pow[col][row]);
			printf("turbine[%i][%i]%f| ", col, row, current_pow[col][row]);
		}
		printf("\n\n");
	}
	printf("\n\n");

	//fprintf(ptr, "max values:\n");

	printf("max values:\n\n");

	for(row = 0; row < ROWCOUNT; row++){
		for(col = 0; col < COLCOUNT; col++){
			//fprintf(ptr, "turbine[%i][%i]%f| ", col, row, max_pow[col][row]);
			printf("turbine[%i][%i]%f| ", col, row, max_pow[col][row]);
		}
		//fprintf(ptr,"\n");
		printf("\n\n");
	}
	printf("\n\n");

	printf("AVERAGE: %f\n\n",new_average());
	printf("-------------------------------------------------------------------------\n\n\n");

	target_vals.current_target++;
}



/*------------------------------------------------------------------
When all the turbines have changed thier value, this will print the results, and then send a broadcast to start the next cycle
--------------------------------------------------------------------*/
void read_results(struct target_vals *rem_requests, FILE *fp){
	while(rem_requests->current_target != rem_requests->end_target){

		pthread_mutex_lock(&controller);

			while(ready_to_print){
				pthread_cond_wait(&controller_var, &controller);
			}

			print_matrix(fp);
			
			//int row;
			//int col;
			/*average = 0;
			for(col = 0; col < COLCOUNT; col++){
				for (row = 0; row < ROWCOUNT; row++){
					average += sum[col][row];
				}
			}*/
			//average /= ROWCOUNT*COLCOUNT;
			//printf("\nAVERAGE: %f\n\n",average);
			ready_to_print = ROWCOUNT * COLCOUNT;
			//rem_requests->current_target++;
			//printf("current_target = %i\n\n", rem_requests->current_target);
			pthread_cond_broadcast(&received_broadcast);
		pthread_mutex_unlock(&controller);
	}
	fclose(fp);
}


//MAIN-----------------------
int main(){
	setpath311('3');
	printf("in_path = %s | out_path = %s\n",in_path, out_path);
	assign_file(current_pow, max_pow, grid, &target_vals, out_path);
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

