	 //store new power into sum variable after performing a series of equations
void calc_sum_average(struct grid_position *turbine){
	float turbine_avg = sum[turbine->col][turbine->row];
	float current_power = current_pow[turbine->col][turbine->row];
	int number_neighbors = num_neighbors[turbine->col][turbine->row];

	turbine_avg += current_power;
	turbine_avg /= number_neighbors + 1;

	int target_value = target_vals.values[target_vals.current_target];
	float max_power = max_pow[turbine->col][turbine->row];	

//The actual equation----------------------------------------------------------------------------------------------
 	if(turbine_avg < target_value){
		    //printf("sum <  | %f [%i][%i]\n", sum[turbine->col][turbine->row], turbine->col, turbine->row);
		    turbine_avg * 1.3 > max_power? turbine_avg = max_power : (turbine_avg *= 1.3);
	}
     else if(turbine_avg > target_value){
	 		//printf("sum >  | %f [%i][%i]\n", sum[turbine->col][turbine->row], turbine->col, turbine->row);
			turbine_avg - (.3 * turbine_avg) < 0 ? turbine_avg = 0 : (turbine_avg *= (1-.3));
	}

	sum[turbine->col][turbine->row] = turbine_avg;
}
