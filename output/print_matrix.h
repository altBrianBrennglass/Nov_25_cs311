
/*------------------------------------------------------------------
Prints the matrix
--------------------------------------------------------------------*/
void print_matrix(FILE *ptr){
	int col;
	int row;


	fprintf(ptr, "\ncurrent_target = %i\n", target_vals.current_target+1);
	fprintf(ptr, "target_value = %.2f\n\n", target_vals.values[target_vals.current_target]);
	fprintf(ptr, "current values:\n");

	for(row = 0; row < ROWCOUNT; row++){
		for(col = 0; col < COLCOUNT; col++){
			fprintf(ptr, "turbine[%i][%i] %.2f| ", col, row, current_pow[col][row]);
		}
		fprintf(ptr,"\n");
	}
	fprintf(ptr,"\n\n");


	fprintf(ptr, "max values:\n");

	for(row = 0; row < ROWCOUNT; row++){
		for(col = 0; col < COLCOUNT; col++){
			fprintf(ptr, "turbine[%i][%i]%.2f| ", col, row, max_pow[col][row]);
		}
		fprintf(ptr, "\n");
	}
	fprintf(ptr, "\n\n");

	fprintf(ptr, "AVERAGE: %f\n\n",new_average());
	fprintf(ptr, "***********************************************\n\n\n");

	target_vals.current_target++;
}
