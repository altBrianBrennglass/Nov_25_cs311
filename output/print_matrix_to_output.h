/*------------------------------------------------------------------
Prints the matrix
--------------------------------------------------------------------*/
void print_matrix(FILE *ptr){
	int col;
	int row;
	//fprintf(ptr, "current values:\n");

	fprintf(ptr, "\ncurrent_target = %i\n\n", target_vals.current_target);
	printf(ptr,"target_value = %f\n\n", target_vals.values[target_vals.current_target]);
	printf(ptr,"current values:\n\n");

	for(row = 0; row < ROWCOUNT; row++){
		for(col = 0; col < COLCOUNT; col++){
			//fprintf(ptr, "turbine[%i][%i] %f| ", col, row, current_pow[col][row]);
			printf(ptr,"turbine[%i][%i]%f| ", col, row, current_pow[col][row]);
		}
		printf(ptr,"\n\n");
	}
	printf(ptr,"\n\n");

	//fprintf(ptr, "max values:\n");

	printf(ptr,"max values:\n\n");

	for(row = 0; row < ROWCOUNT; row++){
		for(col = 0; col < COLCOUNT; col++){
			//fprintf(ptr, "turbine[%i][%i]%f| ", col, row, max_pow[col][row]);
			printf(ptr,"turbine[%i][%i]%f| ", col, row, max_pow[col][row]);
		}
		//fprintf(ptr,"\n");
		printf(ptr,"\n\n");
	}
	printf(ptr,"\n\n");

	printf(ptr,"AVERAGE: %f\n\n",new_average());
	printf(ptr,"-------------------------------------------------------------------------\n\n\n");

	target_vals.current_target++;
}

