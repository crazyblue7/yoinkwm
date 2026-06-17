CC = gcc

debug: 
	$(CC) main.c -o debuggy
	./debuggy
	rm ./debuggy

debugLog: 
	$(CC) main.c -o debuggy
	./debuggy > textloggy.txt

rel:
	$(CC) main.c -o rawWM
