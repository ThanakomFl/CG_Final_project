#include <stdio.h>


struct Pixel
{
	unsigned char b, g, r;
};

struct PixelA
{
	unsigned char b, g, r, a;
};

struct Palette
{
	unsigned char  r, g, b, a;
};

struct Bitmap
{
	short BitCount;
	int Width, Height;
	struct Pixel* pixel;
	struct PixelA* pixelA;
};

static int getByte(FILE *f);
static int get2Byte(FILE *f);
static int get4Byte(FILE *f);
void loadBitmap(Bitmap *bm, const char *filename);