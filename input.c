#include <dos.h>
#include <conio.h>
#include <stdint.h>

#include "i_defs.h"

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
void mouseinit()
{
	regs.w.ax = 0x00;
	int386(0x33, &regs, &regs);

        regs.w.ax = 0x07;
        regs.w.cx = 0;
        regs.w.dx = 361;
        int386(0x33, &regs, &regs);

        regs.w.ax = 0x08;
        regs.w.cx = 0;
        regs.w.dx = 0;
        int386(0x33, &regs, &regs);

        /*regs.w.ax = 0x0F;
        regs.w.cx = 0x10;
        regs.w.dx = 0x20;
        int386(0x33, &regs, &regs);*/
}

void mousemov(Mouse *p)
{
        regs.w.ax = 0x03;
        int386(0x33, &regs, &regs);

        p->x = regs.w.cx;
        p->y = regs.w.dx;
        p->bx = regs.w.bx;
}

