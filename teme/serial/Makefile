run: build
	./serial

build: serial.c
	gcc serial.c ../utils/IO.c -o serial

clean:
	rm -f serial
