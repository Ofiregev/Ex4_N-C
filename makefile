CC = gcc
FLAG = -Wall

all: send meas

meas: measure.c
	$(CC) $(FLAG) -o meas measure.c

send: sender.c
	$(CC) $(FLAG) -o send sender.c

measure.o: measure.c
	$(CC) $(FLAG) -c measure.c

sender.o: sender.c
	$(CC) $(FLAG) -c sender.c

.PHONY: clean all

clean:
	rm -f *.a *.so *.o send meas