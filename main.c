#include <stdio.h>
#include <linux/input.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include "render.h"
#include "input.h"

int main(void) {
	printf("Hello World\n");
	Display scr = initDisplay();
	struct input_event input;
	int rectPos = 120;
	while (1) { // app loop
		int *keys = getKeysPressed(input);
		if ( keys[KEY_D] == 1 ) {
			rectPos = 120;
		} else {
			rectPos = 70;
		}
		clear(scr);
		if ( keys[KEY_ESC] ) {
			//close(scr.fbfd);
			//return 0;
			drawRect(0x00FFFFFF, rectPos,rectPos, 500,500,scr);
		}
		if ( keys[125] && keys[50] ) {
			return 0;
		}
		drawPixel(3, 3, 0x00FF807F, scr);
		drawRect(0x00F99B0E, 20,20, rectPos,rectPos,scr);
		update(scr);
		msleep(7);
	}
	return 0;
}
