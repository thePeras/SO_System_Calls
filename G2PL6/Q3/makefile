CC=gcc
CFLAGS=-I.
DEPS = 
OBJ = tokenring.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

tokenring: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)