#ifndef INPUT_H
#define INPUT_H

#include "display.h"
#include <stdlib.h>

int *getKeysPressed(struct input_event ev, Display scr);
int get_keyboard_event_path(char *out, size_t out_size);

#endif
