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

	for(i=0; i<=s->top; i++) {
		Wall *seg = s->arr[i];
		if(seg->render) {
			mapshift(seg, player, mouse);
		}
	}
}

void playerinput(Coords *player, int angle)
{
	if(keystate[K_W]) {
	        player->x-=(SIN[angle]);
	        player->y-=(COS[angle]);
	}
	if(keystate[K_A]) {
                player->x-=(COS[angle]);
                player->y+=(SIN[angle]);
	}
	if(keystate[K_S]) {
                player->x+=(SIN[angle]);
                player->y+=(COS[angle]);
	}
	if(keystate[K_D]) {
                player->x+=(COS[angle]);
                player->y-=(SIN[angle]);
	}

}

void rotate(int *ptr_x, int *ptr_y, const Coords *p, int angle)
{
	int x = *ptr_x;
	int y = *ptr_y;

	int rotated_x = (int)((x * COS[angle]) - (y * SIN[angle]));
	int rotated_y = (int)((x * SIN[angle]) + (y * COS[angle]));

	*ptr_x = rotated_x;
	*ptr_y = rotated_y;
}

void mapshift(Wall *wall, const Coords *player, const Mouse *mouse)
{
	int x1 = wall->x1;
	int y1 = wall->y1;
	int x2 = wall->x2;
	int y2 = wall->y2;
	int renderw[8];

	int p_x1, p_x2, p_y1, p_y2;

	x1 -= player->x;
	y1 -= player->y;
	x2 -= player->x;
	y2 -= player->y;

	rotate(&x1, &y1, player, mouse->angle);
	rotate(&x2, &y2, player, mouse->angle);

	/*VIEW*/
	if(y1<0&&y2<0) {

		p_x1 = (-x1 << 7) / y1;
		p_x2 = (-x2 << 7) / y2;
		p_y1 = Y_CENTER;
		p_y2 = Y_CENTER;

		p_x1 += X_CENTER;
		p_x2 += X_CENTER;

		renderw[0] = p_x1;
		renderw[1] = p_y1 + (p_y1<<5)/y1;
		renderw[2] = p_x2;
		renderw[3] = p_y2 + (p_y2<<5)/y2;
		renderw[4] = p_x2;
		renderw[5] = p_y2 - (p_y2<<5)/y2;
		renderw[6] = p_x1;
		renderw[7] = p_y1 - (p_y1<<5)/y1;

		polygon(renderw, 4, RED);
	}
}
