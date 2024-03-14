CC=gcc

sddc: sddc.o
	gcc sddc.o -o sddc -lb64

sddc.o: sddc.c
	gcc -c sddc.c

clean:
	rm -rf sddc *.o
