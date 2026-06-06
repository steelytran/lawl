#include <dos.h>
#include <stdlib.h>
#include <conio.h>
#include <stdint.h>

#include "i_defs.h"
#include "colors.h"
#include "draw.h"

union REGS regs;
volatile uint8_t keystate[128];

void __interrupt keyisr()
{
	int key = inp(0x60);
	
	if(key&0x80) {
		keystate[key&0x7f] = 0;
	} else {keystate[key] = 1;}

	outp(0x20, 0x20);
}

int keychar()
{
	regs.h.ah = 0x00;
	int386(0x16, &regs, &regs);
	return regs.h.al;
}

void mouserange(int x, int y)
{
	regs.w.ax = 0x07;
	regs.w.cx = 0;
	regs.w.dx = x;
        int386(0x33, &regs, &regs);

	regs.w.ax = 0x08;
	regs.w.cx = 0;
	regs.w.dx = y;
        int386(0x33, &regs, &regs);
}

void mouseinput(Mouse *p, uint8_t ax)
{
        regs.w.ax = ax;
        int386(0x33, &regs, &regs);

	if(ax == 0x0b) {
		p->x+= 360 - (signed short)regs.w.cx;
		p->x%=360;
		
		if(p->x<1) {p->x += 359;}
	} else{p->x = (signed short)regs.w.cx;}
	
        p->y = (signed short)regs.w.dx;
        p->bx = regs.w.bx;
}

void keyinput(Coords *player, int angle)
{
	int xmov = (keystate[K_A]^keystate[K_D]);
	int ymov = (keystate[K_W]^keystate[K_S]);
	int xyspeed = xmov + ymov - 1;
	int mul = 2>>xyspeed;
	int ds = t1+mul;

        int f_x=ds*-(SIN[angle]);
        int f_y=ds*-(COS[angle]);

        int l_x=ds*-(COS[angle]);
        int l_y=ds*(SIN[angle]);

        int b_x=ds*(SIN[angle]);
        int b_y=ds*(COS[angle]);

        int r_x=ds*(COS[angle]);
        int r_y=ds*-(SIN[angle]);

	if(keystate[K_W]) {
	        player->x += f_x;
	        player->y += f_y;
	} if(keystate[K_A]) {
                player->x += l_x;
                player->y += l_y;
	} if(keystate[K_S]) {
                player->x += b_x;
                player->y += b_y;
	} if(keystate[K_D]) {
                player->x += r_x;
                player->y += r_y;
	}
	/* if(keystate[K_SPACE]) {
                player->z += 1;
	} if(keystate[K_LSHIFT]) {
                player->z -= 1;
	}*/
}
