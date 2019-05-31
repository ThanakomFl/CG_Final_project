#include <stdio.h>
#include "image.h"
#include <stdlib.h>


static int getByte(FILE *f)
{
	return fgetc(f) & 0xFF;
}

static int get2Byte(FILE *f)
{
	return  fgetc(f) | fgetc(f) << 8; 
}

static int get4Byte(FILE *f)
{
	return fgetc(f) | fgetc(f) << 8 | fgetc(f) << 16 | fgetc(f) << 24;
}

void loadBitmap(Bitmap *bm, const char *filename)
{
	//FILE *fp = fopen( filename, "rb");

	FILE *fp;

	fopen_s(&fp, filename, "rb");

	if(!fp){
		fprintf(stderr, "cannot open file: %s\n", filename);
	}

	
	if(get2Byte(fp) != 0x4D42)
	{
		fprintf(stderr, "%s ‚NG\n", filename);
		//exit(0);
	}

	short Planes;//2byte
	int Size, OffBits, infoSize, Compression, Sizeimage,XPelsPerMeter,YPelsPerMeter;
	int ClrUsed, ClrImportant, dummy;

	Size = get4Byte(fp);
	dummy = get4Byte(fp);
	OffBits = get4Byte(fp);//header size
	infoSize = get4Byte(fp);//info size
	bm->Width = get4Byte(fp);
	bm->Height = get4Byte(fp);
	Planes = get2Byte(fp);
	bm->BitCount = get2Byte(fp);
	Compression = get4Byte(fp);
	Sizeimage = get4Byte(fp);
	XPelsPerMeter = get4Byte(fp);
	YPelsPerMeter = get4Byte(fp);
	ClrUsed = get4Byte(fp);
	ClrImportant = get4Byte(fp);

	if(bm->BitCount < 16)
	{
		bm->pixel = (Pixel*)malloc(3 * bm->Width * bm->Height);
		int paletteSize = 1 << bm->BitCount;
		Palette* plt = (Palette*)malloc(sizeof(struct Palette) * paletteSize);
		int i, x, y;
		for(i = 0; i < paletteSize; i++){
			plt[i].b = fgetc(fp);
			plt[i].g = fgetc(fp);
			plt[i].r = fgetc(fp);
			plt[i].a = fgetc(fp);
		}
		if(bm->BitCount == 1)//1bit mono
		{
			int c, d, e, k;
			int n = 0;
			for(y = 0; y < bm->Height; y++)
				for(x = 0; x < bm->Width / 8; x++)
				{
					c = fgetc(fp);
					for(k = 0; k < 8; k++)
					{	
						d = c & 0x80;
						if(d == 0) e = 0;
						else e = 255;
						bm->pixel[n].b = (unsigned char)e;
						bm->pixel[n].g = (unsigned char)e;
						bm->pixel[n].r = (unsigned char)e;
						n++;
						c = c << 1;
					}

				}
		}
		else if(bm->BitCount == 8)//8bit mono or color
		{
			int c;
			

			for(y = 0; y < bm->Height; y++)
				for(x = 0; x < bm->Width; x++)
				{
					c = getByte(fp) ;//& 0xFF;
					Palette p = plt[c];//fgetc(fp)];
					bm->pixel[x + y * bm->Width].b = (unsigned char)p.b;
					bm->pixel[x + y * bm->Width].g = (unsigned char)p.g;
					bm->pixel[x + y * bm->Width].r = (unsigned char)p.r;

				}
		}
	}
	else if(bm->BitCount == 24)//24bit color
	{
		bm->pixel = (Pixel*)malloc(3 * bm->Width * bm->Height);
			
		fread(bm->pixel, 1, 3*bm->Width * bm->Height, fp);
	}

	else//32bit color
	{
		bm->pixelA = (PixelA*)malloc(4 * bm->Width * bm->Height);

		fread(bm->pixelA, 1, 4 * bm->Width * bm->Height, fp);
		fclose(fp);
	}
}
