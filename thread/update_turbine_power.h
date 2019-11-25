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
