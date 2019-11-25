/*------------------------------------------------------------------
Reads file and assigns values to the turbines
--------------------------------------------------------------------*/
void assign_file(float current_pow[COLCOUNT][ROWCOUNT], float max_pow[COLCOUNT][ROWCOUNT], struct grid_position grid_position[COLCOUNT][ROWCOUNT], struct target_vals *targets, char *path){	
	//FILE *fp;
	infile = fopen(path, "r");
	if( infile == NULL){
		printf("could not open file\n");
		exit(EXIT_FAILURE);
	}

	char buf[50];
	const int MAX = 49;
	char *token;
	int col;
	int row;
	int stage = 0;

	while(fgets(buf, MAX, infile) != NULL){	
		char temp_buf[49];
		strcpy(temp_buf, buf);
		token = strtok(temp_buf, " \n");
		while(token != NULL){
			if(token[0] == '\"'){
				stage++;
				col = 0;
				row = 0;
			}
			switch (stage){
				case 1://insert max power into grid
					if(isdigit(token[0])){
						max_pow[col][row] = atof(token);
						grid_position[col][row].col = col;
						grid_position[col][row].row = row;
						++col;
						col %= COLCOUNT;
						!col? row++: 0;
						
					}
					break;
				case 2://insert initial power into grid
					if(isdigit(token[0])){
						current_pow[col][row] = atof(token);
						average += current_pow[col][row];
						++col;
						col %= COLCOUNT;
						!col? row++: 0;
					}
					break;
				case 3://store target values for later use
					if(isdigit(token[0])){
						targets->values[targets->end_target] = atof(token);
						targets->end_target++ ;
					}
					break;
			}
			token = strtok(NULL, " \n");
		}
	}
	fclose(infile);	
	average /= ROWCOUNT *COLCOUNT;

	start_timing();
} 

