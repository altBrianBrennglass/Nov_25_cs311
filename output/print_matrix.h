/*------------------------------------------------------------------
Prints the matrix
--------------------------------------------------------------------*/
void print_matrix(FILE *ptr){
	int col;
	int row;
	//fprintf(ptr, "current values:\n");

	printf("\ncurrent_target = %i\n\n", target_vals.current_target+1);
	printf("target_value = %f\n\n", target_vals.values[target_vals.current_target]);
	printf("current values:\n\n");

	for(row = 0; row < ROWCOUNT; row++){
		for(col = 0; col < COLCOUNT; col++){
			//fprintf(ptr, "turbine[%i][%i] %f| ", col, row, current_pow[col][row]);
			printf("turbine[%i][%i]%f| ", col, row, current_pow[col][row]);
		}
		printf("\n\n");
	}
	printf("\n\n");

	//fprintf(ptr, "max values:\n");

	printf("max values:\n\n");

	for(row = 0; row < ROWCOUNT; row++){
		for(col = 0; col < COLCOUNT; col++){
			//fprintf(ptr, "turbine[%i][%i]%f| ", col, row, max_pow[col][row]);
			printf("turbine[%i][%i]%f| ", col, row, max_pow[col][row]);
		}
		//fprintf(ptr,"\n");
		printf("\n\n");
	}
	printf("\n\n");

	printf("AVERAGE: %f\n\n",new_average());
	printf("-------------------------------------------------------------------------\n\n\n");

	target_vals.current_target++;
}

