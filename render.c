#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include "render.h"
#include <linux/kd.h>

Display nullscr;

void msleep(int millis) {
	usleep(millis*1000);
}
Display initDisplay() {
	Display scr;
	int framebufferfiledescriptor = open("/dev/fb0", O_RDWR);
	int tty_filedescriptor = open("/dev/tty", O_RDWR);
	if ( framebufferfiledescriptor == -1 ) {
		perror("open");
		return nullscr;
	}
	scr.fbfd = framebufferfiledescriptor;
	struct fb_var_screeninfo vinfo;
	ioctl(framebufferfiledescriptor, FBIOGET_VSCREENINFO, &vinfo);
	scr.sizeBytes = vinfo.xres*vinfo.yres*(vinfo.bits_per_pixel / 8);
	scr.width = vinfo.xres;
	scr.height = vinfo.yres;
	int *framebuffer = mmap(NULL,scr.sizeBytes, PROT_READ | PROT_WRITE, MAP_SHARED, framebufferfiledescriptor, 0); 
	if (framebuffer == MAP_FAILED) {
		perror("mmap");
		return nullscr;
	}
	printf("isatty(stdin) = %d\n", isatty(STDIN_FILENO));
	printf("ttyname = %s\n", ttyname(STDIN_FILENO));
	scr.fb = framebuffer;
	scr.bb = malloc(scr.sizeBytes);
	ioctl(tty_filedescriptor, KDSETMODE, KD_TEXT);
	printf("after KDSETMODE\n");
	scr.ttyfd = tty_filedescriptor;
	return scr;
} // got help from chatgpt to get stuff rendering in the initDisplay func
void closeDisplay(Display scr) {
	close(scr.fbfd);
	ioctl(scr.ttyfd, KDSETMODE, KD_TEXT);
}

void drawPixel(int posX, int posY, int color, Display scr) {
	if ( color < 16777216 ) {
		scr.bb[(posY*scr.width)+posX] = color;
	}
}
void drawRect(int color, int x1, int y1, int x2, int y2, Display scr) {
	if ( x2 < x1 || y2 < y1 ) {
		return;
	}
	int height = y2 - y1;
	int width  = x2 - x1;
	for ( int i = 0; i < height; i+=1 ) {
		for ( int j = 0; j < width; j+=1 ) {
			drawPixel(x1+j, y1+i, color, scr);
		}
	}
}
void clear(Display scr){
	for ( int i = 0; i < scr.height; i += 1 ) {
		for ( int j = 0; j < scr.width; j += 1 ) {
			drawPixel(j,   i, 0x00000000, scr);
		}
	}
}
void clearColor(Display scr, int color){
	for ( int i = 0; i < scr.height; i += 1 ) {
		for ( int j = 0; j < scr.width; j += 1 ) {
			// green bg:
			//0x00E0D97B
			drawPixel(j,   i, color, scr);
		}
	}
}
void update(Display scr) {
	memcpy(scr.fb, scr.bb, scr.sizeBytes);
}
