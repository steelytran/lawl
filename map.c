#include <dos.h>
#include <stdlib.h>

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

void pop_render(Stack *s, int index)
{
	s->arr[index] = s->arr[s->top];
	s->top--;
}

void maprender(Stack *s, const Coords *player, const Mouse *mouse)
{
	int i;

	for(i=0; i<=s->top; i++) {
		Wall *seg = s->arr[i];
		mapshift(seg, player, mouse);
	}
}

void playerinput(Coords *player, int angle)
{
	int ds = t1?(t1<<1):(t1+1);

        int f_x=ds*-(SIN[angle]);
        int f_y=ds*-(COS[angle]);

        int l_x=ds*-(COS[angle]);
        int l_y=ds*(SIN[angle]);

        int b_x=ds*(SIN[angle]);
        int b_y=ds*(COS[angle]);

        int r_x=ds*(COS[angle]);
        int r_y=ds*-(SIN[angle]);

	if(keystate[K_W]) {
	        player->x+=f_x;
	        player->y+=f_y;
	} if(keystate[K_A]) {
                player->x+=l_x;
                player->y+=l_y;
	} if(keystate[K_S]) {
                player->x+=b_x;
                player->y+=b_y;
	} if(keystate[K_D]) {
                player->x+=r_x;
                player->y+=r_y;
	}
}

void rotate(int *ptr_x, int *ptr_y, const Coords *p, int angle)
{
	int radius_x = *ptr_x - p->x;
	int radius_y = *ptr_y - p->y;

	*ptr_x = (int)((radius_x * COS[angle]) - (radius_y * SIN[angle]));
	*ptr_y = (int)((radius_x * SIN[angle]) + (radius_y * COS[angle]));
}

void mapshift(Wall *wall, const Coords *player, const Mouse *mouse)
{
	int i;

	int x1 = wall->x1;
	int y1 = wall->y1;
	int x2 = wall->x2;
	int y2 = wall->y2;
	int r_w[8];

	int sx1, sx2, sy1, sy2;

	const int NEARPLANE = -10;

	rotate(&x1, &y1, player, mouse->angle);
	rotate(&x2, &y2, player, mouse->angle);

	/*RELATIVE VIEW*/

	if(y1>=NEARPLANE&&y2>=NEARPLANE) {return;}
	else {

		if(y1>=NEARPLANE) {
			x1 += (int)(NEARPLANE-y1)*(x2-x1)/(y2-y1);
			y1 = NEARPLANE;
		}
		else if(y2>=NEARPLANE) {
			x2 += (int)(NEARPLANE-y2)*(x1-x2)/(y1-y2);
			y2 = NEARPLANE;
		}

		sx1 = (-x1<<7) / y1;
		sx2 = (-x2<<7) / y2;
		sy1 = Y_CENTER;
		sy2 = Y_CENTER;

		sx1 += X_CENTER;
		sx2 += X_CENTER;

		r_w[0] = sx1;
		r_w[1] = sy1 + (sy1<<6)/y1;
		r_w[2] = sx2;
		r_w[3] = sy2 + (sy2<<6)/y2;
		r_w[4] = sx2;
		r_w[5] = sy2 - (sy2<<6)/y2;
		r_w[6] = sx1;
		r_w[7] = sy1 - (sy1<<6)/y1;

		line(r_w[0], r_w[1], r_w[2], r_w[3], wall->color);
		line(r_w[4], r_w[5], r_w[6], r_w[7], wall->color);

		if(y1<NEARPLANE)
			line(r_w[6], r_w[7], r_w[0], r_w[1], wall->color);
		if(y2<NEARPLANE)
			line(r_w[2], r_w[3], r_w[4], r_w[5], wall->color);
	}
}
