CC=gcc
CFLAGS=-I.
DEPS = 
OBJ = txt2epub.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

txt2epub: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)