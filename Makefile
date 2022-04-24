CC := gcc
CFLAGS := -std=c89 -pedantic -Wall -Wextra -g3 -O0
LDFLAGS :=

all: ttd_unpack

ttd_unpack: ttd_unpack.c
	$(CC) $(CFLAGS) $? -o $@ $(LDFLAGS)

clean:
	rm -f ttd_unpack ttd_unpack.exe *.o


.PHONY: clean
