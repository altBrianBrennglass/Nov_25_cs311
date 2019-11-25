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
