#ifndef DRAW_H
#define DRAW_H

extern uint8_t *VGA;
extern uint8_t *VRAM;

void set_mode(uint8_t mode);
void init_tables();
void pixel(int x, int y, uint8_t color);
void line(int x1, int y1, int x2, int y2, uint8_t color);
void polygon(int *vertices, int count, uint8_t color);
void rect(int x1, int y1, int x2, int y2, uint8_t color);
void circle(int x, int y, int radius, uint8_t color);
int edge(int x, int y, int ax, int bx, int ay, int by);
int fill(int *p, uint8_t color);

#endif
