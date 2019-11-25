
/*------------------------------------------------------------------
Finds out a turbine's neighbors, and stores the results into a 2d array
--------------------------------------------------------------------*/
void define_grid_neighbors(int col, int row, int neighbors[COLCOUNT][ROWCOUNT][4][2], int num_neighbors[COLCOUNT][ROWCOUNT]){
        int max_col = --col;
        int max_row = --row;

        for(;col >= 0;col--){
                for(row = max_row; row >= 0; row--){
		          //Northern neighbor
		          neighbors[col][row][0][0] = row == 0 ? -1: (num_neighbors[col][row]++, col);
		          neighbors[col][row][0][1] = row - 1;

		          //Right neighbor
		          neighbors[col][row][1][0] = col + 1 > max_col ? -1 : (num_neighbors[col][row]++, col + 1);
		          neighbors[col][row][1][1] = row;

		          //Southern neighbor
		          neighbors[col][row][2][0] = row + 1 > max_row ? -1 : (num_neighbors[col][row]++, col);
		          neighbors[col][row][2][1] = row + 1;

		          //Left neighbor
		          neighbors[col][row][3][0] = col == 0 ? -1 : (num_neighbors[col][row]++, col - 1);
		          neighbors[col][row][3][1] = row;
			}
			
		}
}

