#ifndef G_COLOR_H
#define G_COLOR_H

enum EGA {
	BLACK = 0,
	BLUE,
	GREEN,
	CYAN,
	RED,
	MAGENTA,
	BROWN,
	WHITE,
	GRAY,
	LIGHTBLUE,
	LIGHTGREEN,
	LIGHTCYAN,
	LIGHTRED,
	LIGHTMAGENTA,
	YELLOW,
	BRIGHTWHITE
};

enum GRAYSCALE {
	G0 = 16,
	G1,
	G2,
	G3,
	G4,
	G5,
	G6,
	G7,
	G8,
	G9,
	GA,
	GB,
	GC,
	GD,
	GE,
	GF
};

enum HUE {
	H16 = 224,
	H17,
	H18,
	H19,
	H20,
	H21,
	H22,
	H23,
	H00,
	H01,
	H02,
	H03,
	H04,
	H05,
	H06,
	H07,
	H08,
	H09,
	H10,
	H11,
	H12,
	H13,
	H14,
	H15,
};

enum SATURATION {
	S0 = 0,
	S1 = 24,
	S3 = 48
};

enum LIGHT {
	L0 = 0,
	L1 = 72,
	L2 = 144
};

int hsl(int hue, int sat, int light);

#endif
