#include <dos.h>
#include <math.h>
#include <stdlib.h>

#include "draw.h"
#include "defs.h"

byte *VRAM = (byte*)0xA0000;
byte *VGA = NULL;

float SIN[360];
float COS[360];
long SIN_ACOS[1024];

void init_tables()
{
	int i;
	
	for(i=0;i<360;i++) {
	        SIN[i]=sin((float)i/180*PI);
	        COS[i]=cos((float)i/180*PI);
	}
	for(i=0;i<1024;i++) {
	        SIN_ACOS[i]=sin(acos((float)i/1024))*0x10000L;
	}
}

void set_mode(byte mode)
{
	union REGS regs;
	regs.w.ax = mode;
	int386(0x10, &regs, &regs);
}

void pixel(int x, int y, byte color)
{
	if(x>=0&&x<320&&y>=0&&y<200) {
		VGA[(y<<8) + (y<<6) + x] = color;
	}
}

void line(int x1, int y1, int x2, int y2, byte color)
{
	int i;
	int dx = x2 - x1;
	int dy = y2 - y1;
	int u_dx = abs(dx);
	int u_dy = abs(dy);
	int sdx = sgn(dx);
	int sdy = sgn(dy);
	int px = u_dx>>1;
	int py = u_dy>>1;
	int x = x1;
	int y = y1;
	
	if(u_dx >= u_dy) {
		for(i = 0; i < u_dx; i++) {
			py+=u_dy;
			if(py>=u_dx) {
				py-=u_dx;
				y+=sdy;
			}
			x+=sdx;
			pixel(x, y, color);
		}
	}
	else {
		for(i = 0; i < u_dy; i++) {
			px+=u_dx;
			if(px>=u_dy) {
				px-=u_dy;
				x+=sdx;
			}
			y+=sdy;
			pixel(x, y, color);
		}
	}
}

void polygon(int *vertices, int count, byte color)
{
	int i;
	int x1, y1, x2, y2;
	for(i=0;i<count-1;i++) {
		x1 = vertices[0+(i<<1)],
		y1 = vertices[1+(i<<1)],
		x2 = vertices[2+(i<<1)],
		y2 = vertices[3+(i<<1)],
		line(x1, y1, x2, y2, color);
	}
	x1 = vertices[0],
	y1 = vertices[1],
	x2 = vertices[-2+(count<<1)],
	y2 = vertices[-1+(count<<1)],
	line(x1, y1, x2, y2, color);
}

void rect(int x1, int y1, int x2, int y2, byte color)
{
	int i, j;
	int dx = x1 - x2;
	int dy = y1 - y2;
	int u_dx = abs(dx);
	int u_dy = abs(dy);
	int sgn_x = sgn(dx);
	int sgn_y = sgn(dy);
	
	int x[2];
	int y[2];
	x[0] = x1; x[1] = x2;
	y[0] = y1; y[1] = y2;

	for(i=0;i<2;i++) {

		for(j=0;j<=u_dx;j++) {
			pixel(x[0]-(j*sgn_x), y[i], color);
		}
		for(j=0;j<=u_dy;j++) {
			pixel(x[i], y[0]-(j*sgn_y), color);
		}
	}
}

void circle(int x, int y, int radius, byte color)
{
	long n=0, invradius=(1/(float)radius)*0x10000L;
	int dx=0, dy=radius-1;
	unsigned short dxoffset, dyoffset, offset = (y<<8)+(y<<6)+x;

	while(dx<=dy) {
	dxoffset = (dx<<8)+(dx<<6);
	dyoffset = (dy<<8)+(dy<<6);
	VGA[offset+dy-dxoffset] = color;
	VGA[offset+dx-dyoffset] = color;
	VGA[offset-dx-dyoffset] = color;
	VGA[offset-dy-dxoffset] = color;
	VGA[offset-dy+dxoffset] = color;
	VGA[offset-dx+dyoffset] = color;
	VGA[offset+dx+dyoffset] = color;
	VGA[offset+dy+dxoffset] = color;
	dx++;
	n+=invradius;
	dy = (int)((radius * SIN_ACOS[(int)(n>>6)]) >> 16);
	}
}
