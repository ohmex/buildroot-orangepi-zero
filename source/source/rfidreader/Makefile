CFLAGS=-I.

LIBS = curl json-c
LINKFLAGS = $(addprefix -l,$(LIBS))

DEPS = rfidreader.h
OBJ = rfidreader.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

rfidreader: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LINKFLAGS)

