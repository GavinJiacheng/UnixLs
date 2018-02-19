all: UnixLs

UnixLs:	Unixls.o
	gcc -o UnixLs Unixls.o

Unixls.o : Unixls.o
	gcc -c Unixls.c

clean:
	rm -f *.o *.s UnixLs
