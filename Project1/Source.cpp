#include <stdio.h>
#include <GL/freeglut.h>   
#include <math.h>



#define PI 3.1415926



void DrawSphere(float radius, int nSlice, int nStack);
void DrawCube();
void DrawTorus(float, float, int, int);
void DrawRevolution(float cs[][2], int nSide, int nSlice);
void DrawSuper(float r, int nSlice, int nStack, double eps1, double eps2);

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(20.0, 0.0, 1.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);




	/*glPushMatrix();
	glTranslatef(0.0, 0.0, 0.0);
	DrawSphere(1.0, 30, 30);
	glPopMatrix();*/

	/*glPushMatrix();
	glTranslatef(0.0, 0.0, -2.0);
	glRotatef(30, 1.0, 0.0, 0.0);
	DrawTorus(0.3, 1.5, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, 2.0);
	glRotatef(-30, 1.0, 0.0, 0.0);
	DrawTorus(0.3, 1.5, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, -3.0, -1.0);
	glRotatef(20, 1.0, 0.0, 0.0);
	DrawTorus(0.3, 1.5, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, -3.0, 1.0);
	glRotatef(-20, 1.0, 0.0, 0.0);
	DrawTorus(0.3, 1.5, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.0);
	DrawSuper(1.25, 30, 30, 0.2, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 2.0, 0.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	DrawTorus(0.3, 0.8, 30, 30);
	glPopMatrix();*/

	float cs[11][2] = { {0.0,2.0},
						{0.5,4.5},
						{0.5,4.0},
						{1.0,3.5},
						{1.0,3.0},
						{1.0,1.0},
						{1.5,1.0},
						{2.0,2.0},
						{2.0,1.5},
						{2.0,1.0},
						{2.5,0.5} }
		;
		
	glPushMatrix();
	glTranslatef(0.0, 0.0, -10.0);
	DrawRevolution(cs, 3, 30);
	glPopMatrix();
	
	glutSwapBuffers();

	glFlush();
}

void init()
{

	glClearColor(0.4, 0.4, 0.8, 1.0);



}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)w / (double)h, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);

}


int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("simple");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	init();
	glutMainLoop();

}

void DrawCube() {
	int i, j;

	GLdouble vertex[][3] = {
	  {-1.0,-1.0,1.0},
	  {1.0,-1.0,1.0},
	  {1.0,1.0,1.0},
	  {-1.0,1.0,1.0},
	  {-1.0,-1.0,-1.0},
	  {1.0,-1.0,-1.0},
	  {1.0,1.0,-1.0},
	  {-1.0,1.0,-1.0}
	};

	int face[][4] = {
	  {0,1,2,3},  // front
	  {1,5,6,2},  // right
	  {5,4,7,6},  // back
	  {4,0,3,7},  // left
	  {4,5,1,0},  // bottom
	  {3,2,6,7}   // top
	};


	for (j = 0; j < 6; j++) {
		glBegin(GL_LINE_LOOP);
		for (i = 0; i < 4; i++) {
			glVertex3dv(vertex[face[j][i]]);
		}
		glEnd();
	}



}

void DrawSphere(float radius, int nSlice, int nStack) {

	int i, j;
	double phi;
	double theta;
	float p[31][31][3];
	float* p1, *p2, *p3, *p4;

	if (nSlice > 30) nSlice = 30;
	if (nStack > 30) nStack = 30;


	// Point: p[i][j][0] = Pij[x], p[i][j][1] = Pij[y], p[i][j][2] = Pij[z]
	for (i = 0; i <= nSlice; i++)
	{
		phi = 2.0 * PI * (double)i / (double)nSlice;
		for (j = 0; j <= nStack; j++)
		{
			theta = PI * (double)j / (double)nStack;
			p[i][j][0] = (float)(radius * sin(theta) * cos(phi));
			p[i][j][1] = (float)(radius * sin(theta) * sin(phi));
			p[i][j][2] = (float)(radius * cos(theta));
		}
	}


	// ---------------- Draw Lines

	//Top(j=0)
	for (i = 0; i < nSlice; i++)
	{
		p1 = p[i][0];	  p2 = p[i][1];
		p3 = p[i + 1][1];
		glBegin(GL_LINE_LOOP);
		glVertex3fv(p1);
		glVertex3fv(p2);
		glVertex3fv(p3);
		glEnd();
	}
	//Bottom
	j = nStack - 1;
	for (i = 0; i < nSlice; i++)
	{
		p1 = p[i][j];	  p2 = p[i][j + 1];
		p3 = p[i + 1][j];
		glBegin(GL_LINE_LOOP);
		glVertex3fv(p1);
		glVertex3fv(p2);
		glVertex3fv(p3);
		glEnd();
	}

	for (i = 0; i < nSlice; i++) {
		for (j = 1; j < nStack - 1; j++)
		{
			p1 = p[i][j];	  p2 = p[i][j + 1];
			p3 = p[i + 1][j + 1]; p4 = p[i + 1][j];
			glBegin(GL_LINE_LOOP);
			glVertex3fv(p1);
			glVertex3fv(p2);
			glVertex3fv(p3);
			glVertex3fv(p4);
			glEnd();
		}

	}
}

void DrawTorus(float radius1, float radius2, int nSide, int nRing)
{
	int i, ii, j, jj;
	double phi, ph1, ph2;
	double theta, th1, th2;
	float rr[31], zz[31];
	float p[1000][3];

	if (radius1 > radius2) {
		printf("radius1 < radius2\n ");
		return;
	}

	if (nSide * nRing > 2000) {
		printf("The number of points must <= 2000\n ");
		return;
	}

	if (nRing > 30)
		nRing = 30;
	if (nSide > 30)
		nSide = 30;

	for (j = 0; j < nSide; j++)
	{
		theta = PI - 2.0 * PI * (double)j / (double)nSide;
		rr[j] = (float)(radius2 + radius1 * (float)cos(theta));
		zz[j] = (float)(radius1 * (float)sin(theta));
	}
	for (i = 0; i < nRing; i++)
	{
		phi = 2.0 * PI * (double)i / (double)nRing;
		for (j = 0; j < nSide; j++)
		{
			p[j * nRing + i][0] = rr[j] * (float)cos(phi);
			p[j * nRing + i][1] = rr[j] * (float)sin(phi);
			p[j * nRing + i][2] = zz[j];
		}
	}

	double s1, s2;

	for (i = 0; i < nRing; i++)
	{
		ii = i + 1;
		if (ii == nRing) ii = 0;
		s1 = (double)i / (double)nRing; s2 = (double)(ii) / (double)nRing;
		ph1 = 2.0 * PI * s1;
		ph2 = 2.0 * PI * s2;
		for (j = 0; j < nSide; j++)
		{
			jj = j + 1;
			if (jj == nSide) jj = 0;
			th1 = PI - 2.0 * PI * (double)j / (double)nSide;
			th2 = PI - 2.0 * PI * (double)jj / (double)nSide;
			glBegin(GL_LINE_LOOP);
			glVertex3fv(p[j * nRing + i]);
			glVertex3fv(p[jj * nRing + i]);
			glVertex3fv(p[jj * nRing + ii]);
			glVertex3fv(p[j * nRing + ii]);
			glEnd();
		}
	}
}

void DrawRevolution(float cs[][2], int nSide, int nSlice)
{
	int i, ii, j, jj;
	double phi;
	float p[31][31][3];

	if (nSide > 30) {
		printf("nSide < 30\n");
		return;
	}

	if (nSlice > 30)
		nSlice = 30;

	int nSide2;

	if (cs[0][0] == 0.0 && cs[nSide - 1][0] == 0.0)
		nSide2 = nSide - 1;
	else
		nSide2 = nSide;


	for (i = 0; i < nSlice; i++)
	{
		phi = 2.0 * PI * (double)i / (double)nSlice;

		for (j = 0; j < nSide; j++)
		{
			p[i][j][0] = cs[j][0] * (float)cos(phi);
			p[i][j][1] = cs[j][0] * (float)sin(phi);
			p[i][j][2] = cs[j][1];
		}
	}

	for (i = 0; i < nSlice; i++)
	{
		ii = i + 1;

		if (ii == nSlice)
			ii = 0;

		for (j = 0; j < nSide2; j++)
		{
			jj = j + 1;

			if (jj == nSide)
				jj = 0;

			glBegin(GL_LINE_LOOP);
			glVertex3fv(p[i][j]);
			glVertex3fv(p[i][jj]);
			glVertex3fv(p[ii][jj]);
			glVertex3fv(p[ii][j]);
			glEnd();
		}
	}
}



void DrawSuper(float r, int nSlice, int nStack, double eps1, double eps2)
{

	int i, j, ip;
	int k1, k2;
	double ct, theta, phi, z, cc;
	float pd[31 * 31][3];

	if (nSlice > 30) nSlice = 30;
	if (nStack > 30) nStack = 30;

	for (j = 0; j <= nStack; j++)
	{
		theta = (PI / (double)nStack) * ((double)nStack / 2.0 - (double)j);

		if (theta >= 0.0)
		{
			if (theta == 0.0)
				z = 0.0;//pow(a,b)
			else
				z = pow(sin(fabs(theta)), eps1);//z
		}
		else
		{
			z = -pow(sin(fabs(theta)), eps1);
		}
		for (i = 0; i <= nSlice / 2; i++)
		{
			k1 = nSlice * j + i;
			k2 = nSlice * j + nSlice - i;
			phi = 2.0 * PI * (double)i / (double)nSlice;
			ct = cos(phi);
			if (ct == 0.0) cc = 0.0;
			else if (ct > 0) { cc = pow(ct, eps2); }
			else { cc = -pow(fabs(ct), eps2); }
			if (j == 0 || j == nStack)
			{
				pd[k1][0] = 0.0f;
				pd[k1][1] = 0.0f;
			}

			else
			{
				pd[k1][0] = r * (float)(pow(cos(theta), eps1) * cc);
				if (sin(phi) == 0.0) pd[k1][1] = 0.0f;
				else pd[k1][1] = r * (float)(pow(cos(theta), eps1) * pow(fabs(sin(phi)), eps2));
			}
			if (i == 0) k2 = k1;
			pd[k2][0] = pd[k1][0];
			pd[k2][1] = -pd[k1][1];
			pd[k1][2] = r * (float)z;
			pd[k2][2] = r * (float)z;
		}
	}


	for (i = 0; i < nSlice; i++)
	{
		ip = i + 1;
		if (ip == nSlice) ip = 0;
		for (j = 0; j < nStack; j++)
		{
			glBegin(GL_LINE_LOOP);
			glVertex3fv(pd[i + j * nSlice]);
			glVertex3fv(pd[i + (j + 1) * nSlice]);
			glVertex3fv(pd[ip + (j + 1) * nSlice]);
			glVertex3fv(pd[ip + j * nSlice]);
			glEnd();
		}
	}
}

