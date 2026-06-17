CC = gcc
CODE = main.c render.c input.c
CCFLAGDEBUG = -g -O0

debug: $(CODE)
	$(CC) $(CODE) -o debuggy $(CCFLAGDEBUG)
	./debuggy

debugLog: $(CODE)
	$(CC) $(CODE) -o debuggy $(CCFLAGDEBUG)
	./debuggy > textloggy.txt

rel: $(CODE)
	$(CC) $(CODE) -o yoinkWM
