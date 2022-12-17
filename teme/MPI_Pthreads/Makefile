run: build
	mpirun --oversubscribe -np 2 ./mpi_pthreads

build: mpi_pthreads.c
	mpicc mpi_pthreads.c ../utils/IO.c -o mpi_pthreads -lpthread

clean:
	rm -f mpi_pthreads
