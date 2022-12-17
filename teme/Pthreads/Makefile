run: build
	./pthreads

build: pthreads.c
	gcc pthreads.c ../utils/IO.c -o pthreads -lpthread

clean:
	rm -f pthreads
