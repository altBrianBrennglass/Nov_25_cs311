
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
