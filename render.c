#include <dos.h>
#include <stdlib.h>

#include "i_defs.h"
#include "colors.h"
#include "draw.h"

#define PARENT(x) = (x-1)>>1
#define FRONT(x) = (x<<1)+1
#define BACK(x) = (x<<1)+2

extern volatile uint8_t keystate[];

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

	int color; /*testing*/

	const int NEARPLANE = -10;

	rotate(&x1, &y1, player, mouse->x);
	rotate(&x2, &y2, player, mouse->x);

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
		r_w[1] = sy1 + (sy1<<7)/y1;
		r_w[2] = sx2;
		r_w[3] = sy2 + (sy2<<7)/y2;
		r_w[4] = sx2;
		r_w[5] = sy2 - (sy2<<7)/y2;
		r_w[6] = sx1;
		r_w[7] = sy1 - (sy1<<7)/y1;

		color = (sx1>sx2)?RED:BLUE;

		line(r_w[0], r_w[1], r_w[2], r_w[3], color);
		line(r_w[4], r_w[5], r_w[6], r_w[7], color);

		if(y1<NEARPLANE)
			line(r_w[6], r_w[7], r_w[0], r_w[1], color);
		if(y2<NEARPLANE)
			line(r_w[2], r_w[3], r_w[4], r_w[5], color);
	}
}
