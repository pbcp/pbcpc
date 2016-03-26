CFLAGS=-O3 -s
LDLIBS=-lcurl
TARGET=pbc pbp

all: $(TARGET)

clean:
	$(RM) $(TARGET)

%: %.c
	$(CC) -o $@ $(CFLAGS) $< $(LDLIBS)
