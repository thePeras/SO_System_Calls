CC=gcc
CFLAGS=-I.
DEPS = 
OBJ = samples.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

samples: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)