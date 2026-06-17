CC = gcc
CODE = main.c render.c input.c

debug: 
	$(CC) $(CODE) -o debuggy
	./debuggy
	rm ./debuggy

debugLog: 
	$(CC) $(CODE) -o debuggy
	./debuggy > textloggy.txt

rel:
	$(CC) $(CODE) -o rawWM
