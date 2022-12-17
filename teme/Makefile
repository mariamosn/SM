build: ./serial/serial.c ./MPI/mpi.c ./MPI_OpenMP/mpi_omp.c \
		./MPI_Pthreads/mpi_pthreads.c ./OpenMP/omp.c ./Pthreads/pthreads.c
	gcc ./serial/serial.c ./utils/IO.c -o ./serial/serial
	mpicc ./MPI/mpi.c ./utils/IO.c -o ./MPI/mpi
	mpicc ./MPI_OpenMP/mpi_omp.c ./utils/IO.c -o ./MPI_OpenMP/mpi_omp -fopenmp
	mpicc ./MPI_Pthreads/mpi_pthreads.c ./utils/IO.c -o \
		./MPI_Pthreads/mpi_pthreads -lpthread
	gcc ./OpenMP/omp.c ./utils/IO.c -o ./OpenMP/omp -fopenmp
	gcc ./Pthreads/pthreads.c ./utils/IO.c -o ./Pthreads/pthreads -lpthread

run_serial:
	cd ./serial; \
	./serial

build_serial: ./serial/serial.c
	gcc ./serial/serial.c ./utils/IO.c -o ./serial/serial

run_mpi:
	cd ./MPI; \
	mpirun --oversubscribe -np 4 ./mpi

build_mpi: ./MPI/mpi.c
	mpicc ./MPI/mpi.c ./utils/IO.c -o ./MPI/mpi

run_mpi_omp:
	cd ./MPI_OpenMP; \
	mpirun --oversubscribe -np 2 ./mpi_omp

build_mpi_omp: ./MPI_OpenMP/mpi_omp.c
	mpicc ./MPI_OpenMP/mpi_omp.c ./utils/IO.c -o ./MPI_OpenMP/mpi_omp -fopenmp

run_mpi_pthreads:
	cd ./MPI_Pthreads; \
	mpirun --oversubscribe -np 2 ./mpi_pthreads

build_mpi_pthreads: ./MPI_Pthreads/mpi_pthreads.c
	mpicc ./MPI_Pthreads/mpi_pthreads.c ./utils/IO.c -o \
		./MPI_Pthreads/mpi_pthreads -lpthread

run_omp:
	cd ./OpenMP; \
	./omp

build_omp: ./OpenMP/omp.c
	gcc ./OpenMP/omp.c ./utils/IO.c -o ./OpenMP/omp -fopenmp

run_pthreads:
	cd ./Pthreads; \
	./pthreads

build_pthreads: ./Pthreads/pthreads.c
	gcc ./Pthreads/pthreads.c ./utils/IO.c -o ./Pthreads/pthreads -lpthread

clean:
	rm -f ./serial/serial ./MPI/mpi ./MPI_OpenMP/mpi_omp \
		./MPI_Pthreads/mpi_pthreads ./OpenMP/omp ./Pthreads/pthreads
