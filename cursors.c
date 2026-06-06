#include <dos.h>
#include "i_defs.h"
#include "draw.h"
#include "colors.h"

int cursor_1[] = {
0b00000000,
0b00000000,
0b00000000,
0b00001000,
0b00011000,
0b00111000,
0b00011000,
0b00100000};

int crosshair[] = {
0b00000000,
0b00000000,
0b00001000,
0b00010100,
0b00001000,
0b00000000,
0b00000000,
0b00000000};

void cursor(Mouse *m, int* cursor)
{
	int i, j;
	int x, y;
	x = m->x;
	y = m->y;

	for(i=0; i<8; i++) {
		for(j=0; j<8; j++) {
			if(cursor[i] & 1<<j){
				pixel(x+j-3, y+i-3, WHITE);
			}
		}
	}
}
