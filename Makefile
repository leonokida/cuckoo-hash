# Leon Okida e Pedro Willian

CFLAGS = -lm -Wall -std=c99 # compilation flag

all: myht

myht: myht.c
	gcc myht.c -o myht $(CFLAGS)
clean:
	-rm -f myht