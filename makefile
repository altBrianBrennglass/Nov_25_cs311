run:
	gcc -Wall -g course_fns.o main.c -o main -lpthread
	./main

run_full:
	gcc -Wall -g course_fns.o full_main.c -o full_main -lpthread
	./full_main

clean:
	rm main
	rm full_main
