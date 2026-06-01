#include <dos.h>

#include "i_defs.h"
#include "g_defs.h"
#include "draw.h"

extern volatile uint8_t keystate[];

void initstack(Stack *s)
{
	s->top = -1;
}

void push_render(Stack *s, Wall *w)
{
	if(s->top < MAXLINE-1) {
		s->arr[++(s->top)] = w;
	}
}

void maprender(Stack *s, const Coords *player, const Mouse *mouse)
{
	int i;
	Wall *seg = s->arr[s->top];
	if(seg->render) {
		mapshift(seg, player, mouse);
	}
}

void playerinput(Coords *player, const Mouse *mouse)
{
	if(keystate[K_W]){ --player->y;}
	if(keystate[K_A]){ --player->x;}
	if(keystate[K_S]){ ++player->y;}
	if(keystate[K_D]){ ++player->x;}
}

void mapshift(Wall *wall, const Coords *player, const Mouse *mouse)
{
	int x1 =  -player->x - wall->x1;
	int y1 =  -player->y - wall->y1;
	int x2 =  -player->x - wall->x2;
	int y2 =  -player->y - wall->y2;

	x1+=159;
	y1+=99;
	x2+=159;
	y2+=99;

	line(x1, y1, x2, y2, wall->color);
}
