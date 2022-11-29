run: build
	./lab06

build: lab06.c
	export OMP_NUM_THREADS=5
	gcc lab06.c -o lab06 -fopenmp

clean:
	rm -rf ./lab06
