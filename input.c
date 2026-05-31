#include <dos.h>
#include <conio.h>

#include "defs.h"

union REGS regs;
volatile byte lastkey = 0;

void __interrupt keyisr()
{
	lastkey = inp(0x60);
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
        __asm {
                mov ax, 07h
                mov cx, 0
                mov dx, 319
                int 33h

                mov ax, 08h
                mov cx, 0
                mov dx, 199
                int 33h

                mov ax, 0Fh
                mov cx, 10h
                mov dx, 20h
                int 33h
        }
}

void mousemov(Mouse *p)
{
        regs.w.ax = 0x03;
        int386(0x33, &regs, &regs);

        p->x = regs.w.cx;
        p->y = regs.w.dx;
        p->bx = regs.w.bx;
}

