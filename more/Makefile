CC=gcc -std=c99

test: common.c sha1.c base64.c utf8.c test.c utf8.c fields.c
	$(CC) -o $@ -I .. -Dmore_test $^

clean:
	rm -f test *.o
