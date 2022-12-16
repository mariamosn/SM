run: build
	mpirun --oversubscribe -np 4 ./mpi

build: mpi.c
	mpicc mpi.c ../utils/IO.c -o mpi

clean:
	rm -f mpi
