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
	if(keystate[K_W]) {
	        player->x-=(SIN[mouse->x]);
	        player->y-=(COS[mouse->x]);
	}
	if(keystate[K_A]) {
                player->x-=(COS[mouse->x]);
                player->y+=(SIN[mouse->x]);
	}
	if(keystate[K_S]) {
                player->x+=(SIN[mouse->x]);
                player->y+=(COS[mouse->x]);
	}
	if(keystate[K_D]) {
                player->x+=(COS[mouse->x]);
                player->y-=(SIN[mouse->x]);
	}

}

void rotate(int *ptr_x, int *ptr_y, Coords *p, Mouse *m)
{
	int angle = m->x;

	int x = *ptr_x;
	int y = *ptr_y;

	*ptr_x = p->x + (int)((x * COS[angle]) - (y * SIN[angle]));
	*ptr_y = p->y + (int)((x * SIN[angle]) + (y * COS[angle]));
}


void mapshift(Wall *wall, const Coords *player, const Mouse *mouse)
{
	int x1 = wall->x1;
	int y1 = wall->y1;
	int x2 = wall->x2;
	int y2 = wall->y2;

	Coords player_rel;
	
	player_rel.x = 159 - player->x;
	player_rel.y = 99 - player->y;

	rotate(&x1, &y1, &player_rel, mouse);
	rotate(&x2, &y2, &player_rel, mouse);
	
	line(x1, y1, x2, y2, wall->color);
}
