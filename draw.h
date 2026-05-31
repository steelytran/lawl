#ifndef DRAW_H
#define DRAW_H

#include "defs.h"
extern byte *VGA;
extern byte *VRAM;

void set_mode(byte mode);
void init_tables();
void pixel(int x, int y, byte color);
void line(int x1, int y1, int x2, int y2, byte color);
void polygon(int *vertices, int count, byte color);
void rect(int x1, int y1, int x2, int y2, byte color);
void circle(int x, int y, int radius, byte color);

#endif
