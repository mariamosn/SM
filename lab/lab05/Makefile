run: build
	mpirun --oversubscribe -np 4 lab05

build: lab05.c
	mpicc lab05.c -o lab05

clean:
	rm -rf ./lab05
