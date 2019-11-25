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

