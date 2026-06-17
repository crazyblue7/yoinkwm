#ifndef DISPLAY_H
#define DISPLAY_H
typedef struct {
	int width;
	int height;
	int size;
	int sizeBytes;
	int fbfd;
	int *fb;
	int *bb;
	int ttyfd;
} Display;
#endif
