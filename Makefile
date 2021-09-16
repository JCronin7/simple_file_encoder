CC=gcc
CFLAGS=-I.
CFLAGS+=-lpthread
DEPS = encrypt.h
OBJ = encrypt.o main.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

encrypt: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm $(OBJ)