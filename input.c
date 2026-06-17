#include <linux/input.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "input.h"

int *getKeysPressed(struct input_event ev, Display scr) {
	int *keys = malloc(KEY_MAX+1);
	ssize_t n = read(scr.fbfd, &ev, sizeof(ev));
	if (n != sizeof(struct input_event)) {
		printf("perror");
		return keys;
	}

	if ( ev.type == EV_KEY ) {
		if ( ev.value == 1 ) {
			keys[ev.code] = 1;
		} else if ( ev.value == 0 ) {
			keys[ev.code] = 0;
		}
		printf("code %i\n",ev.code);
	}
	// type codes:
	// 1:press
	// 2:release
	// 3:repeat (the repeating from holding the key)
	return keys;
}

// the function below was chatgpt cuz... i dont know how to do thingy
int get_keyboard_event_path(char *out, size_t out_size) {
	FILE *f = fopen("/proc/bus/input/devices", "r");
	if (!f) return -1;

	char line[512];
	char name[256] = {0};
	int found = 0;

	char event[64] = {0};

	while (fgets(line, sizeof(line), f)) {

		// blank line = new device block
		if (line[0] == '\n') {
			name[0] = 0;
			event[0] = 0;
			found = 0;
			continue;
		}

		// device name
		if (strncmp(line, "N: Name=", 9) == 0) {
			sscanf(line, "N: Name=\"%255[^\"]\"", name);

			if (strcmp(name, "keyd virtual keyboard") == 0) {
				found = 1;
			}
		}

		// handlers line (contains eventX)
		if (found && strstr(line, "H: Handlers=")) {
			char *ev = strstr(line, "event");
			if (ev) {
				sscanf(ev, "event%63s", event);
			}
		}
	}

	fclose(f);

	if (event[0] == 0) return -1;

	snprintf(out, out_size, "/dev/input/event%s", event);
	return 0;
}
