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

