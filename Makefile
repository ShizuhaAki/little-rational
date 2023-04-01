CC ?= gcc
CFLAGS += -Wall -I. -fPIC

test: tap.c test.c

check: test
	./test
	
clean:
	rm ./test