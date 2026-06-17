#ifndef RENDER_H
#define RENDER_H

#include "display.h"

void msleep(int millis);
Display initDisplay();

void drawPixel(int posX, int posY, int color, Display scr);
void drawRect(int color, int x1, int y1, int x2, int y2, Display scr);
void clear(Display scr);
void clearColor(Display scr, int color);
void update(Display scr);

#endif
