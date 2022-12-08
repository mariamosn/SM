all: build
	./lab08 < test1.in > new_test1.out
	./lab08 < test2.in > new_test2.out
	./lab08 < test3.in > new_test3.out

	diff new_test1.out test1.out > diff1 || echo 'Test 1 failed'
	diff new_test2.out test2.out > diff2 || echo 'Test 2 failed'
	diff new_test3.out test3.out > diff3 || echo 'Test 3 failed'

run: build
	./lab08

build: lab08.cpp
	g++ lab08.cpp -o lab08

clean:
	rm -rf ./lab08 new_test* diff*
