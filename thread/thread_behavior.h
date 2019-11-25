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

