CC = gcc

CFLAGS = -Wall -O3
LDFLAGS = -lpthread com.c msg_queue.c

TARGETS = matmul nqueen echo-server echo-client matmul-th nqueen-th

all: $(TARGETS)

matmul: matmul.o
nqueen: nqueen.o
matmul-th: matmul-th.o
nqueen-th: nqueen-th.o
echo-server: echo-server.o
echo-client: echo-client.o

matmul.o: matmul.c 
nqueen.o: nqueen.c 
matmul-th.o: matmul-th.c
nqueen-th.o: nqueen-th.c
echo-server.o: echo-server.c
echo-client.o: echo-client.c

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm *.o

distclean:
	rm $(TARGETS)
allclean:
	make clean
	make distclean
