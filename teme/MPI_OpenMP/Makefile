run: build
	mpirun --oversubscribe -np 2 ./mpi_omp

build: mpi_omp.c
	mpicc mpi_omp.c ../utils/IO.c -o mpi_omp -fopenmp

clean:
	rm -f mpi_omp
