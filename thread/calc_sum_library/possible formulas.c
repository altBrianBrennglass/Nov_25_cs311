v1.h:

 	if(turbine_avg < target_value){
		    //printf("sum <  | %f [%i][%i]\n", sum[turbine->col][turbine->row], turbine->col, turbine->row);
		    turbine_avg * 1.3 > max_pow? turbine_avg = max_pow : turbine_avg *= 1.3;
	}
     else if(turbine_avg > target_value){
	 		//printf("sum >  | %f [%i][%i]\n", sum[turbine->col][turbine->row], turbine->col, turbine->row);
			turbine_avg - (.3 * turbine_avg) < 0 ? turbine_avg = 0 : turbine_avg *= (1-.3);
	}
