# Leon Augusto Okida Gonçalves

CFLAGS = -lm -Wall -std=c99 # compilation flag

all: myht

myht: myht.c
	gcc myht.c -o myht $(CFLAGS)
clean:
	-rm -f myht