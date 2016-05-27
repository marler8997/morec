CC=gcc -std=c99 -I ../common

arps: arps.c ../common/string_util.o ../common/maclib.o
	${CC} -o arps arps.c ../common/string_util.o ../common/maclib.o

clean:
	rm -f *~ *.o arps