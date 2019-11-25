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

