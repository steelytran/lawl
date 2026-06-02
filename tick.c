#include <dos.h>
#include <conio.h>
#include <stdint.h>

void __interrupt tick()
{

	outp(0x20, 0x20);
}

void tickrate(int div)
{
	outp(0x43, 0x36);
	outp(0x40, div & 0xFF);
	outp(0x40, (div>>8)&0xFF);
}
