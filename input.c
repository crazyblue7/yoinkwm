#include <linux/input.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#include "input.h"

// the function below was chatgpt ctrlc ctrlv cuz... i dont know how to do thingy
int get_keyboard_event_path(char *out, size_t out_size) {
	FILE *f = fopen("/proc/bus/input/devices", "r");
	if (!f) {
		perror("fopen");
		return -1;
	}

	char line[512];
	char name[256] = {0};
	int found = 0;

	char event[64] = {0};

	while (fgets(line, sizeof(line), f)) {

		// blank line = new device block
		if (line[0] == '\n' && !found ) {
			printf("blank line: found=%d event='%s'\n", found, event);
			name[0] = 0;
			event[0] = 0;
			found = 0;
			continue;
		}

		// device name
		if (strncmp(line, "N: Name=", 8) == 0) {
			sscanf(line, "N: Name=\"%255[^\"]\"", name);

			if (strstr(name, "keyd") != NULL && strstr(name, "keyboard")) {
				printf("NAME: yoink found[%s]\n", name);
				found = 1;
			}
		}

		// handlers line (contains eventX)
		if (found && strstr(line, "H: Handlers=")) {
			printf("hand %s\n", line);
			char *ev = strstr(line, "event");
			if (ev) {
				sscanf(ev, "event%63s", event);
				printf("ev '%s'\n", event);
			}
		}
	}

	fclose(f);

	if (event[0] == 0) return -1;

	snprintf(out, out_size, "/dev/input/event%s", event);
	return 0;
}
// now kinda chatgpt but not ctrl c ctrl v
int *getKeysPressed(struct input_event ev) {
	char path[128];
	int *keys = malloc((KEY_MAX+1)*sizeof(int));
	int fd;

	int ret = get_keyboard_event_path(path, sizeof(path));
	printf("ret = %d\n", ret);
	if (ret == 0) {
		fd = open(path, O_RDONLY);
		printf("fd = %d\n", fd);

		if (fd == -1) {
			perror("open");
		}
		printf("Using: %s\n", path);
	} else {
		return 0;
	}
	ssize_t n = read(fd, &ev, sizeof(ev));
	if (n != sizeof(struct input_event)) {
		printf("perror");
		return keys;
	}

	printf("type=%u code=%u value=%d\n",
		ev.type,ev.code,ev.value);
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

