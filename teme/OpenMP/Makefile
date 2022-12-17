run: build
	./omp

build: omp.c
	gcc omp.c ../utils/IO.c -o omp -fopenmp

clean:
	rm -f omp
