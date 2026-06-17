#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <linux/input.h>

typedef struct {
	int width;
	int height;
	int size;
	int sizeBytes;
	int fbfd;
	int *fb;
	int *bb;
} Display;

Display nullscr;

void print(char string[]) {
	printf("%s\n",string);
}
void msleep(int millis) {
	usleep(millis*1000);
}
Display initDisplay() {
	Display scr;
	int framebufferfiledescriptor = open("/dev/fb0", O_RDWR);
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
	scr.fb = framebuffer;
	scr.bb = malloc(scr.sizeBytes);
	return scr;
} // got help from chatgpt to get stuff rendering in the initDisplay func

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
void keysPressed(struct input_event ev) {
	ssize_t n = read(fd, &ev, sizeof(ev));
	if (n == (ssize_t)sizeof(ev)) 
}

int main(void) {
	print("Hello World");
	Display scr = initDisplay();
	struct input_event input;
	while (1) { // app loop
		clear(scr);
		drawPixel(3, 3, 0x00FF807F, scr);
		drawRect(0x00F99B0E, 20,20,120,120,scr);
		update(scr);
		msleep(7);
		if(1){

		}
	}
	sleep(5);
	close(scr.fbfd);
}
