#include <linux/input.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "input.h"

int *getKeysPressed(struct input_event ev, Display scr) {
	int *keys = malloc(KEY_MAX+1);
	ssize_t n = read(scr.fbfd, &ev, sizeof(ev));
	if ( ev.type == EV_KEY ) {
		if ( ev.value == 1 ) {
			keys[ev.code] = 1;
			printf("code %i\n",ev.code);
		} else if ( !ev.value ) {
			keys[ev.code] = 0;
		}
	}
	// type codes:
	// 1:press
	// 2:release
	// 3:repeat (the repeating from holding the key)
	return keys;
}
