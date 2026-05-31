#include <dos.h>
#include "defs.h"
#include "draw.h"
#include "colors.h"

int cursor1[] = {
0b00000001,
0b00000011,
0b00000111,
0b00001111,
0b00011111,
0b00111111,
0b00001100,
0b00001000};

int crosshair[] = {
0b00011000,
0b00011000,
0b00011000,
0b11100111,
0b11100111,
0b00011000,
0b00011000,
0b00011000};

void cursor(Mouse *m, int* cursor)
{
	int i, j;
	int x, y;
	x = m->x;
	y = m->y;

	for(i=0; i<8; i++) {
		for(j=0; j<8; j++) {
			if(cursor[i] & 1<<j){
				pixel(x+j, y+i, WHITE);
			}
		}
	}
}
