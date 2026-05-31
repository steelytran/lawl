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

void mousemov(int *x, int *y)
{
	static int yaw=0;

	regs.w.ax = 0x0B;
	int386(0x33, &regs, &regs);

	yaw+=(short)regs.w.cx;

	yaw %= 360;
	if (yaw<0){yaw+=360;}

	*x = yaw;
}
