build: crcw crew secv

crcw: crcw_pram.c
	gcc crcw_pram.c -lpthread -o crcw

crew: crew_pram.c
	gcc crew_pram.c -lpthread -o crew

secv: secv.c
	gcc secv.c -o secv

test_secv_2:
	./secv < in

test_secv_3:
	./secv < in2

test_crcw_2:
	./crcw < in

test_crcw_3:
	./crcw < in2

test_crew_2:
	./crew < in

test_crew_3:
	./crew < in2

test_2:
	./secv < in
	./crcw < in
	./crew < in
	cat out

test_3:
	./secv < in2
	./crcw < in2
	./crew < in2
	cat out2

clean:
	rm -rf secv crcw crew