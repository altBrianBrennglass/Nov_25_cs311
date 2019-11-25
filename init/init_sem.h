/*------------------------------------------------------------------
initializes sempahors
--------------------------------------------------------------------*/
void init_sem(sem_t sem[COLCOUNT][ROWCOUNT]){
	int col;
	int row;
	for(row = 0; row < ROWCOUNT; row++){
		for(col = 0; col < COLCOUNT; col++){
			sem_init(&sem[col][row], 0, 1);
		}
	}
}

