#include <GL/freeglut.h>
#include <math.h>
#include "myPrimitive.h"  
#include "myTexture.h"
#include "image.h"
#include "load_bmp.h"



#define vectorInnerProduct(v,q) \
	((v)[0] * (q)[0] + \
	(v)[1] * (q)[1] + \
	(v)[2] * (q)[2])	


/* a = b x c */

#define vectorCrossProduct(a,b,c) \
	(a)[0] = (b)[1] * (c)[2] - (c)[1] * (b)[2]; \
	(a)[1] = (b)[2] * (c)[0] - (c)[2] * (b)[0]; \
	(a)[2] = (b)[0] * (c)[1] - (c)[0] * (b)[1];



#define skyboxImageWidth 1024
#define skyboxImageHeight 1024

float angle = 0.0, deltaAngle = 0.0;

float eyeX = 0.0, eyeY = 1.5, eyeZ = 30.0;

float objX = 0.0, objY = 0.0, objZ = -1.0;

int deltaMove = 0, bla = 0;


//---------- ตัวแปรเก็บข้อมูลสีของ texture ----------- 

GLubyte texImage128[16][128][128][4];
GLubyte texImage256[16][256][256][4];
GLubyte texImage512[16][512][512][4];
GLubyte texImage1024[16][1024][1024][4];

GLuint texID[16];






static float mat_ambient[7][4] = {
	{ 0.0, 0.0, 0.0, 1.0 },			        // Black Plastic
	{ 0.329412, 0.223529, 0.027451, 1.0 },	//  Brass		
	{ 0.2125,0.1275,0.054,1.0},			  // Bronze
	{0.25,0.25,0.25,1.0},					  //Chrome
	{0.191125,0.0735,0.0225,1.0},			  //Copper
	{0.24725,0.1995,0.0745,1.0},				  //Gold
	{1.0,1.0,1.0,1.0}
};
static float mat_diffuse[7][4] = {
	{ 0.01, 0.01, 0.01, 1.0 },
	{ 0.780392, 0.568627, 0.113725, 1.0 },
	{0.714,0.4284,0.18144,1.0},
	{0.4,0.4,0.4,1.0},
	{0.7038,0.27048,0.0828,1.0},
	{0.75164,0.60648,0.22648,1.0},
	{1.0,1.0,1.0,1.0}
};

static float mat_specular[7][4] = {
	{ 0.5, 0.5, 0.5, 1.0 },
	{0.992157, 0.941176, 0.807843, 1.0 },
	{0.393548,0.271906,0.166721,1.0},
	{0.774597,0.774597,0.774597,1.0},
	{0.256777,0.137622,0.086014,1.0},
	{0.628281,0.555802,0.366065,1.0},
	{1.0,1.0,1.0,1.0}
};

static float mat_shininess[7][1] = {
	{ 32.0 },
	{ 27.8974  },
	{ 25.6 },
	{ 76.8 },
	{ 12.8},
	{ 51.2 },
	{1.0}
};

void orientMe(float ang) {
	objX = sin(ang);
	objZ = -cos(ang);
}

void DCube(float x, float y, float z, int tec, float size) {
	glBindTexture(GL_TEXTURE_2D, texID[tec]); // เลือก texture ที่ต้องการ
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(-90.0, 1, 0, 0);
	drawTexCube(size);
	glPopMatrix();
}

void DCylinder(float x, float y, float z, float rb, float rt, float hi, int tec) {
	glBindTexture(GL_TEXTURE_2D, texID[tec]); // เลือก texture ที่ต้องการ
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(-90.0, 1, 0, 0);
	drawTexCylinder(rb, rt, hi, 50, 50);
	glPopMatrix();
}

void DCylinderWR(float d, int xx, int yy, int zz, float x, float y, float z, float rb, float rt, float hi, int tec) {
	glBindTexture(GL_TEXTURE_2D, texID[tec]); // เลือก texture ที่ต้องการ
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(d, xx, yy, zz);
	glRotatef(-90.0, 1, 0, 0);
	drawTexCylinder(rb, rt, hi, 50, 50);
	glPopMatrix();
}

void DS(float x, float y, float z, float r, int tec) {
	glPushAttrib(GL_LIGHTING_BIT);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient[1]);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse[1]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular[1]);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess[1]);

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texID[tec]);  // เลือก texture ที่ต้องการ
	glPushMatrix();
	glTranslatef(x, y, z);
	drawTexSphere(r, 30, 30);
	//drawSphere(1.0,30,30);
	glPopMatrix();

	glPopAttrib();
}

void moveMeFlat(int i) {
	eyeX = eyeX + i * (objX)*0.1;
	eyeZ = eyeZ + i * (objZ)*0.1;
}

void Lookup(int i) {
	objY += 0.01*i;
}

void setTexture128(int no, int w, int h)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glBindTexture(GL_TEXTURE_2D, texID[no]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texImage128[no]);


	// glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
   // glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // repeat sky
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void setTexture1024(int no, int w, int h)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glBindTexture(GL_TEXTURE_2D, texID[no]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texImage1024[no]);

	// glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	// glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, 0);
}


void makeTexImage128(Bitmap *bm, int no, int w, int h)
{
	int i, j, ii, jj;

	if (bm->Width < w) printf("NG\n");

	for (j = 0; j < h; j++)
	{
		jj = j * (int)((float)bm->Height / (float)h);
		for (i = 0; i < w; i++)
		{
			ii = i * (int)((float)bm->Width / (float)w);
			if (bm->BitCount <= 24)
			{
				texImage128[no][j][i][0] = (bm->pixel[ii + jj * bm->Width]).r;
				texImage128[no][j][i][1] = (bm->pixel[ii + jj * bm->Width]).g;
				texImage128[no][j][i][2] = (bm->pixel[ii + jj * bm->Width]).b;
				texImage128[no][j][i][3] = (unsigned char)255;
			}
			else
			{
				texImage128[no][j][i][0] = (bm->pixelA[ii + jj * bm->Width]).r;
				texImage128[no][j][i][1] = (bm->pixelA[ii + jj * bm->Width]).g;
				texImage128[no][j][i][2] = (bm->pixelA[ii + jj * bm->Width]).b;
				texImage128[no][j][i][3] = (bm->pixelA[ii + jj * bm->Width]).a;
			}
		}
	}
}

void makeAlphaImage128(Bitmap *bm, int no, int w, int h)
{
	int i, j, ii, jj;

	if (bm->Width < w) printf("NG\n");

	for (j = 0; j < h; j++)
	{
		jj = j * (int)((float)bm->Height / (float)h);
		for (i = 0; i < w; i++)
		{
			ii = i * (int)((float)bm->Width / (float)w);
			if (bm->BitCount <= 24)
			{
				texImage128[no][j][i][0] = (bm->pixel[ii + jj * bm->Width]).r;
				texImage128[no][j][i][1] = (bm->pixel[ii + jj * bm->Width]).g;
				texImage128[no][j][i][2] = (bm->pixel[ii + jj * bm->Width]).b;
				if (texImage128[no][j][i][0] <= 10 && texImage128[no][j][i][1] <= 240 && texImage128[no][j][i][2] <= 240)
					texImage128[no][j][i][3] = 0;
				else
					texImage128[no][j][i][3] = 255;

			}
			else
			{
				texImage128[no][j][i][0] = (bm->pixelA[ii + jj * bm->Width]).r;
				texImage128[no][j][i][1] = (bm->pixelA[ii + jj * bm->Width]).g;
				texImage128[no][j][i][2] = (bm->pixelA[ii + jj * bm->Width]).b;
				if (texImage128[no][j][i][0] <= 10 && texImage128[no][j][i][1] <= 10 && texImage128[no][j][i][2] <= 10)
					texImage128[no][j][i][3] = 51;
				else
					texImage128[no][j][i][3] = 255;
			}
		}
	}
}


void makeTexImage1024(Bitmap *bm, int no, int w, int h)
{
	int i, j, ii, jj;

	if (bm->Width < w) printf("NG\n");

	for (j = 0; j < h; j++)
	{
		jj = j * (int)((float)bm->Height / (float)h);
		for (i = 0; i < w; i++)
		{
			ii = i * (int)((float)bm->Width / (float)w);
			if (bm->BitCount <= 24)
			{
				texImage1024[no][j][i][0] = (bm->pixel[ii + jj * bm->Width]).r;
				texImage1024[no][j][i][1] = (bm->pixel[ii + jj * bm->Width]).g;
				texImage1024[no][j][i][2] = (bm->pixel[ii + jj * bm->Width]).b;
				texImage1024[no][j][i][3] = (unsigned char)255;
			}
			else
			{
				texImage1024[no][j][i][0] = (bm->pixelA[ii + jj * bm->Width]).r;
				texImage1024[no][j][i][1] = (bm->pixelA[ii + jj * bm->Width]).g;
				texImage1024[no][j][i][2] = (bm->pixelA[ii + jj * bm->Width]).b;
				texImage1024[no][j][i][3] = (bm->pixelA[ii + jj * bm->Width]).a;
			}
		}
	}
}



void makeSkybox(float ss) {




	glEnable(GL_TEXTURE_2D);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);

	// Just in case we set all vertices to white.
	glColor4f(1, 1, 1, 1);


	glBindTexture(GL_TEXTURE_2D, texID[2]);  // north = texID[2]
	glPushMatrix();
	glTranslatef(0.0, 0.0, -ss / 2.0);
	glRotatef(90.0, 1, 0, 0);
	drawTexPlate(ss);
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, texID[3]);  // east
	glPushMatrix();
	glTranslatef(ss / 2.0, 0.0, 0.0);
	glRotatef(-90.0, 0, 1, 0);
	glRotatef(90.0, 1, 0, 0);
	drawTexPlate(ss);
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, texID[4]);  //west
	glPushMatrix();
	glTranslatef(-ss / 2.0, 0.0, 0.0);
	glRotatef(90.0, 0, 1, 0);
	glRotatef(90.0, 1, 0, 0);
	drawTexPlate(ss);
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, texID[5]);  //south
	glPushMatrix();
	glTranslatef(0.0, 0.0, ss / 2.0);
	glRotatef(-180.0, 0, 1, 0);
	glRotatef(90.0, 1, 0, 0);
	drawTexPlate(ss);
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, texID[6]);  //up
	glPushMatrix();
	glTranslatef(0.0, ss / 2.0, 0.0);
	glRotatef(180.0, 0, 0, 1);
	drawTexPlate(ss);
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, texID[7]);  //down
	glPushMatrix();
	glTranslatef(0.0, -ss / 2.0, 0.0);
	glRotatef(180.0, 0, 1, 0);
	drawTexPlate(ss);
	glPopMatrix();

	glPopAttrib();

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
}

void vectorNormalize(float *v) {
	float d = (sqrt((v[0] * v[0]) + (v[1] * v[1]) + (v[2] * v[2])));
	v[0] = v[0] / d;
	v[1] = v[1] / d;
	v[2] = v[2] / d;
}

void Billboard(float *cam, float *worldPos) {

	float lookAt[3] = { 0,0,1 }, objToCamProj[3], up[3], angleCosine;

	objToCamProj[0] = cam[0] - worldPos[0];
	objToCamProj[1] = 0;
	objToCamProj[2] = cam[2] - worldPos[2];



	vectorNormalize(objToCamProj);



	vectorCrossProduct(up, lookAt, objToCamProj);


	angleCosine = vectorInnerProduct(lookAt, objToCamProj);


	if ((angleCosine < 0.99990) && (angleCosine > -0.9999))
		glRotatef(acos(angleCosine) * 180 / 3.14, up[0], up[1], up[2]);
}


void  display(void)
{

	float size = 500.0f;
	float pos[3];

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	if (deltaMove)
		moveMeFlat(deltaMove);

	if (deltaAngle) {
		angle += deltaAngle;
		orientMe(angle);
	}
	float cam[3] = { eyeX,eyeY,eyeZ };

	glLoadIdentity();
	gluLookAt(eyeX, eyeY, eyeZ, eyeX + objX, eyeY + objY, eyeZ + objZ, 0.0f, 1.0f, 0.0f);



	makeSkybox(size);

	glPushAttrib(GL_LIGHTING_BIT);
	glPushMatrix();
	drawFloor(size, size, 100, 100);
	glPopMatrix();
	glPopAttrib();



	/*
//---------------------------- Sphere ------------------------

	glPushAttrib(GL_LIGHTING_BIT);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient[1]);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse[1]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular[1]);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess[1]);

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texID[1]);  // เลือก texture ที่ต้องการ
	glPushMatrix();
	glTranslatef(0.0,1.0,0.0);
	drawTexSphere(1.0,30,30);
	//drawSphere(1.0,30,30);
	glPopMatrix();

	glPopAttrib();
	*/

	//---------------------------- Cylinder ------------------------
		// cage cow
	for (int i = 1; i < 13; i++) {
		DCube(i*4, 0.0, 0.0, 1, 0.7);
		DCube(i*4, 0.7, 0.0, 1, 0.7);
		DCube(4, 0.0, -i * 4, 1, 0.7);
		DCube(4, 0.7, -i * 4, 1, 0.7);
		DCube(48, 0.0, -i * 4, 1, 0.7);
		DCube(48, 0.7, -i * 4, 1, 0.7);
		DCube(i * 4, 0.0, -48, 1, 0.7);
		DCube(i * 4, 0.7, -48, 1, 0.7);
	}
	for (int i = 4; i < 49; i++) {
		DCube(i, 1.4, 0.0, 1, 0.7);
		DCube(i, 1.4, -48, 1, 0.7);
		DCube(4, 1.4, -i, 1, 0.7);
		DCube(48, 1.4, -i, 1, 0.7);
	}

	// cage cow
	for (int i = 1; i < 13; i++) {
		DCube(-i * 4, 0.0, 0.0, 1, 0.7);
		DCube(-i * 4, 0.7, 0.0, 1, 0.7);
		DCube(-4, 0.0, -i * 4, 1, 0.7);
		DCube(-4, 0.7, -i * 4, 1, 0.7);
		DCube(-48, 0.0, -i * 4, 1, 0.7);
		DCube(-48, 0.7, -i * 4, 1, 0.7);
		DCube(-i * 4, 0.0, -48, 1, 0.7);
		DCube(-i * 4, 0.7, -48, 1, 0.7);
	}
	for (int i = 4; i < 49; i++) {
		DCube(-i, 1.4, 0.0, 1, 0.7);
		DCube(-i, 1.4, -48, 1, 0.7);
		DCube(-4, 1.4, -i, 1, 0.7);
		DCube(-48, 1.4, -i, 1, 0.7);
	}

	// cage cow
	for (int i = 2; i < 13; i++) {
		DCube(i * 4, 0.0, 8, 1, 0.7);
		DCube(i * 4, 0.7, 8, 1, 0.7);
		DCube(4, 0.0, i * 4, 1, 0.7);
		DCube(4, 0.7, i * 4, 1, 0.7);
		DCube(48, 0.0, i * 4, 1, 0.7);
		DCube(48, 0.7, i * 4, 1, 0.7);
		DCube(i * 4, 0.0, 48, 1, 0.7);
		DCube(i * 4, 0.7, 48, 1, 0.7);
	}
	for (int i = 8; i < 49; i++) {
		DCube(i, 1.4, 8, 1, 0.7);
		DCube(i, 1.4, 48, 1, 0.7);
		DCube(4, 1.4, i, 1, 0.7);
		DCube(48, 1.4, i, 1, 0.7);
	}

	// cage cow
	for (int i = 2; i < 13; i++) {
		DCube(-i * 4, 0.0, 8, 1, 0.7);
		DCube(-i * 4, 0.7, 8, 1, 0.7);
		DCube(-4, 0.0, i * 4, 1, 0.7);
		DCube(-4, 0.7, i * 4, 1, 0.7);
		DCube(-48, 0.0, i * 4, 1, 0.7);
		DCube(-48, 0.7, i * 4, 1, 0.7);
		DCube(-i * 4, 0.0, 48, 1, 0.7);
		DCube(-i * 4, 0.7, 48, 1, 0.7);
	}
	for (int i = 8; i < 49; i++) {
		DCube(-i, 1.4, 8, 1, 0.7);
		DCube(-i, 1.4, 48, 1, 0.7);
		DCube(-4, 1.4, i, 1, 0.7);
		DCube(-48, 1.4, i, 1, 0.7);
	}

	// rua
	for (int i = -75; i <= 75; i++) {
		DCube(75, 0.0, i, 13, 0.5);
		DCube(75, 0.5, i+0.5, 13, 0.5);
		DCube(75, 1, i, 13, 0.5);
		DCube(75, 1.5, i + 0.5, 13, 0.5);
		DCube(75, 2, i, 13, 0.5);
		DCube(75, 2.5, i + 0.5, 13, 0.5);
		DCube(75, 3, i, 13, 0.5);
		DCube(75, 3.5, i + 0.5, 13, 0.5);
		DCube(75, 4, i, 1, 0.5);
		DCube(75, 4.5, i + 0.5, 13, 0.5);
		DCube(75, 5, i , 13, 0.5);
	}

	for (int i = -75; i <= 75; i++) {
		DCube(-75, 0.0, i, 13, 0.5);
		DCube(-75, 0.5, i + 0.5, 13, 0.5);
		DCube(-75, 1, i, 13, 0.5);
		DCube(-75, 1.5, i + 0.5, 13, 0.5);
		DCube(-75, 2, i, 13, 0.5);
		DCube(-75, 2.5, i + 0.5, 13, 0.5);
		DCube(-75, 3, i, 13, 0.5);
		DCube(-75, 3.5, i + 0.5, 13, 0.5);
		DCube(-75, 4, i, 13, 0.5);
		DCube(-75, 4.5, i + 0.5, 13, 0.5);
		DCube(-75, 5, i, 13, 0.5);
	}

	for (int i = -75; i <= 75; i++) {
		DCube(i, 0.0, -75, 13, 0.5);
		DCube(i+0.5, 0.5, -75, 13, 0.5);
		DCube(i, 1, -75, 13, 0.5);
		DCube(i + 0.5, 1.5, -75, 13, 0.5);
		DCube(i, 2, -75, 13, 0.5);
		DCube(i + 0.5, 2.5, -75, 13, 0.5);
		DCube(i, 3, -75, 13, 0.5);
		DCube(i + 0.5, 3.5, -75, 13, 0.5);
		DCube(i, 4.0, -75, 13, 0.5);
		DCube(i + 0.5, 4.5, -75, 13, 0.5);
		DCube(i, 5.0,-75, 13, 0.5);
	}

	for (int i = 10; i <= 75; i++) {
		DCube(i, 0.0, 75, 13, 0.5);
		DCube(i + 0.5, 0.5, 75, 13, 0.5);
		DCube(i, 1, 75, 13, 0.5);
		DCube(i + 0.5, 1.5, 75, 13, 0.5);
		DCube(i, 2, 75, 13, 0.5);
		DCube(i + 0.5, 2.5, 75, 13, 0.5);
		DCube(i, 3, 75, 13, 0.5);
		DCube(i + 0.5, 3.5, 75, 13, 0.5);
		DCube(i, 4.0, 75, 13, 0.5);
		DCube(i + 0.5, 4.5, 75, 13, 0.5);
		DCube(i, 5.0, 75, 13, 0.5);
	}

	for (int i = 10; i <= 75; i++) {
		DCube(-i, 0.0, 75, 13, 0.5);
		DCube(-i + 0.5, 0.5, 75, 13, 0.5);
		DCube(-i, 1, 75, 13, 0.5);
		DCube(-i + 0.5, 1.5, 75, 13, 0.5);
		DCube(-i, 2, 75, 13, 0.5);
		DCube(-i + 0.5, 2.5, 75, 13, 0.5);
		DCube(-i, 3, 75, 13, 0.5);
		DCube(-i + 0.5, 3.5, 75, 13, 0.5);
		DCube(-i, 4.0, 75, 13, 0.5);
		DCube(-i + 0.5, 4.5, 75, 13, 0.5);
		DCube(-i, 5.0, 75, 13, 0.5);
	}

	for (int i = 0; i < 11 ; i++) {
		DCube(9.5, i*0.5, 75, 13, 0.5);
		DCube(-9.5, i*0.5, 75, 13, 0.5);
	}

	for (int i = 0; i <=18; i++) {
		DCube(i-9, 5.5, 75, 13, 0.5);
		DCube(9-i, 5.5, 75, 13, 0.5);
		DCube(i - 9.5, 5.5, 75, 13, 0.5);
		DCube(9.5 - i, 5.5, 75, 13, 0.5);
	}

	DCylinder(8.5, 3, 75, 1, 1.5, 6, 1);
	DCylinder(-8.5, 3, 75, 1, 1.5, 6, 1);
	DCylinder(0, 3, 75, 1, 1.5, 6, 1);


	//---------------------------- ชิงช้าสวรรค์ ------------------------
	glBindTexture(GL_TEXTURE_2D, texID[14]); // เลือก texture ที่ต้องการ
	glPushMatrix();
	glTranslatef(0.0, 10.0, -60);
	drawTexTorus(0.5, 6.0, 20, 20);
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, texID[14]); // เลือก texture ที่ต้องการ
	glPushMatrix();
	glTranslatef(0.0, 10.0, -60);
	drawTexTorus(0.5, 3.0, 20, 20);
	glPopMatrix();
	DS(0.0, 10.0, -60, 2.0, 15);
	DS(0.0, 17.5, -60, 1.0, 15);
	DS(-7.5, 10.0, -60, 1.0, 15);
	DS(7.5, 10.0, -60, 1.0, 15);
	DS(0.0, 2.5, -60, 1.0, 15);
	DS(-5, 4.5, -60, 1.0, 15);
	DS(5, 4.5, -60, 1.0, 15);
	DS(-5, 15.5, -60, 1.0, 15);
	DS(5, 15.5, -60, 1.0, 15);

	DCylinderWR(45, 0, 0, 1, 0.0, 10, -60, 0.5, 0.5, 12, 14);
	DCylinderWR(-45, 0, 0, 1, 0.0, 10, -60, 0.5, 0.5, 12, 14);

	DCylinderWR(30, 0, 0, 1, 0, 8, -61, 0.5, 0.5, 12, 7);
	DCylinderWR(-30, 0, 0, 1, -2.0, 5, -61, 0.5, 0.5, 12, 7);


	//---------------------------- cow ------------------------
	//---------------------

	glAlphaFunc(GL_GREATER, 0.2);
	glEnable(GL_ALPHA_TEST);

	for (int i = 0; i < 5; i++)
		for (int j = 1; j < 6; j++) {
			glPushMatrix();
			glTranslatef(5 + i * 10.0, 0.0, 5 + -j * 10.0);
			pos[0] = 5 + i * 10.0; pos[1] = 0; pos[2] = 5 + -j * 10.0;
			Billboard(cam, pos);
			glTranslatef(0.0, 1.5, 0.0);
			glRotatef(90.0, 1, 0, 0);
			glBindTexture(GL_TEXTURE_2D, texID[9]);
			drawTexPlate(3.0);
			glPopMatrix();
		}

	for (int i = 1; i < 6; i++)
		for (int j = 1; j < 6; j++) {
			glPushMatrix();
			glTranslatef(5 + -i * 10.0, 0.0, 5 + -j * 10.0);
			pos[0] = 5 + -i * 10.0; pos[1] = 0; pos[2] = 5 + -j * 10.0;
			Billboard(cam, pos);
			glTranslatef(0.0, 1.5, 0.0);
			glRotatef(90.0, 1, 0, 0);
			glBindTexture(GL_TEXTURE_2D, texID[11]);
			drawTexPlate(3.0);
			glPopMatrix();
		}

	for (int i = 0; i < 5; i++)
		for (int j = 1; j < 5; j++) {
			glPushMatrix();
			glTranslatef(5 + i * 10.0, 0.0, 5 + j * 10.0);
			pos[0] = 5 + i * 10.0; pos[1] = 0; pos[2] = 5 + j * 10.0;
			Billboard(cam, pos);
			glTranslatef(0.0, 1.5, 0.0);
			glRotatef(90.0, 1, 0, 0);
			glBindTexture(GL_TEXTURE_2D, texID[12]);
			drawTexPlate(3.0);
			glPopMatrix();
		}

	for (int i = 1; i < 6; i++)
		for (int j = 1; j < 5; j++) {
			glPushMatrix();
			glTranslatef(5 + -i * 10.0, 0.0, 5 + j * 10.0);
			pos[0] = 5 + -i * 10.0; pos[1] = 0; pos[2] = 5 + j * 10.0;
			Billboard(cam, pos);
			glTranslatef(0.0, 1.5, 0.0);
			glRotatef(90.0, 1, 0, 0);
			glBindTexture(GL_TEXTURE_2D, texID[10]);
			drawTexPlate(3.0);
			glPopMatrix();
		}

	for (int i = -8; i < 8; i++)
		for (int j = -7; j < 7; j++) {
			if (i % 2 == 0) {
				glPushMatrix();
				glTranslatef(5 + i * 10.0, 0.0, 5 + -j * 10.0);
				pos[0] = 5 + i * 10.0; pos[1] = 0; pos[2] = 5 + -j * 10.0;
				Billboard(cam, pos);
				glTranslatef(1.8, 1.5, 3.5);
				glRotatef(90.0, 1, 0, 0);
				glBindTexture(GL_TEXTURE_2D, texID[0]);
				drawTexPlate(3.0);
				glPopMatrix();
			}
		}

	for (int i = -8; i < 8; i++)
		for (int j = -7; j < 7; j++) {
			if (j == 0 || i == 0) {
				glPushMatrix();
				glTranslatef(5 + i * 10.0, 0.0, 5 + -j * 10.0);
				pos[0] = 5 + i * 10.0; pos[1] = 0; pos[2] = 5 + -j * 10.0;
				Billboard(cam, pos);
				if (i%2==0) {
					glTranslatef(-3, 1.5, 0);
				}
				else {
					glTranslatef(-4.5, 1.5, 1.3);
				}

				glRotatef(90.0, 1, 0, 0);
				glBindTexture(GL_TEXTURE_2D, texID[8]);
				drawTexPlate(3.0);
				glPopMatrix();
			}
		}

	for (int i = -8; i < 8; i++)
		for (int j = -7; j < 7; j++) {
			if (j == 0 || i == 0) {
				glPushMatrix();
				glTranslatef(5 + i * 10.0, 0.0, 5 + -j * 10.0);
				pos[0] = 5 + i * 10.0; pos[1] = 0; pos[2] = 5 + -j * 10.0;
				Billboard(cam, pos);
				if (j % 3 == 0) {
					glTranslatef(-7.5, 1.5, -1.5);
				}else if (j % 5 == 0) {
					glTranslatef(-3.5, 1.5, -1.5);
				}
				else {
					glTranslatef(-7.5, -1.5, -1.5);
				}

				glRotatef(90.0, 1, 0, 0);
				glBindTexture(GL_TEXTURE_2D, texID[16]);
				drawTexPlate(3.0);
				glPopMatrix();
			}
		}


	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
	//--------------
	glFlush();
	glutSwapBuffers();
}



void  initEnvironment(void)
{
	float  light0_position[] = { 10.0, 10.0, 10.0, 1.0 };
	float  light0_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	float  light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	float  light0_ambient[] = { 1.0, 1.0, 1.0, 1.0 };


	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);



	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);


	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);


	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);

	// -------------Texture ------------------

	glGenTextures(15, texID);



	Bitmap* imageBMP = new Bitmap();
	loadBitmap(imageBMP, "bmp/new_brick.bmp");
	makeTexImage128(imageBMP, 1, 128, 128);
	setTexture128(1, 128, 128);



	// ---- ---------------- Skybox Texture ------------------------------------
	Bitmap* skyboxBMP = new Bitmap();

	loadBitmap(skyboxBMP, "bmp/reef_north.bmp");
	//loadBitmap(skyboxBMP, "bmp/reef_north.bmp");
	makeTexImage1024(skyboxBMP, 2, skyboxImageWidth, skyboxImageHeight);
	setTexture1024(2, skyboxImageWidth, skyboxImageHeight);


	loadBitmap(skyboxBMP, "bmp/reef_east.bmp");
	//loadBitmap(skyboxBMP, "bmp/reef_east.bmp"); 
	makeTexImage1024(skyboxBMP, 3, skyboxImageWidth, skyboxImageHeight);
	setTexture1024(3, skyboxImageWidth, skyboxImageHeight);


	loadBitmap(skyboxBMP, "bmp/reef_west.bmp");
	//loadBitmap(skyboxBMP, "bmp/reef_west.bmp"); 
	makeTexImage1024(skyboxBMP, 4, skyboxImageWidth, skyboxImageHeight);
	setTexture1024(4, skyboxImageWidth, skyboxImageHeight);


	loadBitmap(skyboxBMP, "bmp/reef_south.bmp");
	//loadBitmap(skyboxBMP, "bmp/reef_south.bmp"); 
	makeTexImage1024(skyboxBMP, 5, skyboxImageWidth, skyboxImageHeight);
	setTexture1024(5, skyboxImageWidth, skyboxImageHeight);


	loadBitmap(skyboxBMP, "bmp/reef_up.bmp");
	//loadBitmap(skyboxBMP, "bmp/reef_up.bmp"); 
	makeTexImage1024(skyboxBMP, 6, skyboxImageWidth, skyboxImageHeight);
	setTexture1024(6, skyboxImageWidth, skyboxImageHeight);


	loadBitmap(skyboxBMP, "bmp/reef_down.bmp");
	//loadBitmap(skyboxBMP, "bmp/reef_down.bmp"); 
	makeTexImage1024(skyboxBMP, 7, skyboxImageWidth, skyboxImageHeight);
	setTexture1024(7, skyboxImageWidth, skyboxImageHeight);

	// ---- ---------------- Tree ------------------------------------
	Bitmap* treeBMP = new Bitmap();
	loadBitmap(treeBMP, "bmp/tree3.bmp");
	makeAlphaImage128(treeBMP, 8, 128, 128);
	setTexture128(8, 128, 128);

	Bitmap* cow = new Bitmap();
	loadBitmap(cow, "bmp/cow1.bmp");
	makeAlphaImage128(cow, 9, 128, 128);
	setTexture128(9, 128, 128);

	Bitmap* zeebra = new Bitmap();
	loadBitmap(zeebra, "bmp/fox.bmp");
	makeAlphaImage128(zeebra, 10, 128, 128);
	setTexture128(10, 128, 128);

	Bitmap* Lion = new Bitmap();
	loadBitmap(Lion, "bmp/lion.bmp");
	makeAlphaImage128(Lion, 11, 128, 128);
	setTexture128(11, 128, 128);

	// ---- ---------------- Tree ------------------------------------
	Bitmap* Bear = new Bitmap();
	loadBitmap(Bear, "bmp/bear.bmp");
	makeAlphaImage128(Bear, 12, 128, 128);
	setTexture128(12, 128, 128);

	Bitmap* grass = new Bitmap();
	loadBitmap(grass, "bmp/grass.bmp");
	makeAlphaImage128(grass, 0, 128, 128);
	setTexture128(0, 128, 128);

	Bitmap* gr = new Bitmap();
	loadBitmap(gr, "bmp/new_brick2.bmp");
	makeAlphaImage128(gr, 13, 128, 128);
	setTexture128(13, 128, 128);

	Bitmap* gre = new Bitmap();
	loadBitmap(gre, "bmp/metal.bmp");
	makeAlphaImage128(gre, 14, 128, 128);
	setTexture128(14, 128, 128);

	Bitmap* grenn = new Bitmap();
	loadBitmap(grenn, "bmp/brick3.bmp");
	makeAlphaImage128(grenn, 15, 128, 128);
	setTexture128(15, 128, 128);

	Bitmap* boy = new Bitmap();
	loadBitmap(boy, "bmp/man4.bmp");
	makeAlphaImage128(boy, 16, 128, 128);
	setTexture128(16, 128, 128);

	glClearColor(0.5, 0.5, 0.8, 0.0);

}

void  reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (double)w / h, 1, 1000);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, eyeZ, eyeX + objX, eyeY + objY, eyeZ + objZ, 0.0f, 1.0f, 0.0f);
}



void pressKey(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_LEFT: deltaAngle = -0.05f; break;
	case GLUT_KEY_RIGHT: deltaAngle = 0.05f; break;
	case GLUT_KEY_UP: deltaMove = 4; break;
	case GLUT_KEY_DOWN: deltaMove = -4; break;

	}
}

void releaseKey(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_LEFT:
	case GLUT_KEY_RIGHT: deltaAngle = 0.0f; break;
	case GLUT_KEY_UP:
	case GLUT_KEY_DOWN: deltaMove = 0; break;

	}
}

void  idle(void)
{
	glutPostRedisplay();
}

int  main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL);
	glutInitWindowSize(640, 640);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Sample Texture");

	initEnvironment();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);


	glutMainLoop();
	return 0;
}