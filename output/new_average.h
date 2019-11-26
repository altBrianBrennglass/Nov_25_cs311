float new_average(){
	int col;
	int row;
	average = 0;
	if(target_vals.current_target)
		for(col = 0; col < COLCOUNT; col++){
			for (row = 0; row < ROWCOUNT; row++){
				average += sum[col][row];
			}
		}
	else
		for(col = 0; col < COLCOUNT; col++){
				for (row = 0; row < ROWCOUNT; row++){
					average += current_pow[col][row];
				}
			}
	average /= ROWCOUNT*COLCOUNT;
	return average ;
}//> target_vals.values[target_vals.current_target] ? average *= .7 : (average *= 1.3)
