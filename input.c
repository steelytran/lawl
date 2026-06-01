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

void mousemov(Mouse *p)
{
        regs.w.ax = 0x0b;
        int386(0x33, &regs, &regs);

	p->x = (signed short)regs.w.cx;
        p->y = (signed short)regs.w.dx;
        p->bx = regs.w.bx;

	p->angle += p->x;
	p->angle%=360;
	if(p->angle<1){p->angle += 359;}
}

