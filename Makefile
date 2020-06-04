
all: client

client:	client.c
	gcc -o client client.c -I.

clean:
	rm -rf *.o *~ client