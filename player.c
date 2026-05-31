#include <dos.h>

#include "defs.h"
#include "draw.h"
#include "colors.h"

extern volatile uint8_t lastkey;

void playerinput(Coords *player, const Mouse *mouse)
{
	switch(lastkey) {
		case K_W: --player->y; break;
		case K_A: --player->x; break;
		case K_S: ++player->y; break;
		case K_D: ++player->x; break;
	}
}

/*
void mapshift(int *wall, const Coords *player, const Mouse *mouse)
{
	wall[0] += player->x;
	wall[1] += player->y;
	wall[2] += player->x;
	wall[3] += player->y;
}
*/

void mapshift(int *wall, const Coords *player, const Mouse *mouse)
{

	int x1 =  -player->x - wall[0];
	int y1 =  -player->y - wall[1];
	int x2 =  -player->x - wall[2];
	int y2 =  -player->y - wall[3];

	x1+=159;
	y1+=99;
	x2+=159;
	y2+=99;

	line(x1, y1, x2, y2, GREEN);
	pixel(159, 99, RED);
}
