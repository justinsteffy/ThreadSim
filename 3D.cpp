
#include <gl/glaux.h>
#include <gl/glut.h>
#include <math.h>
#include "Ball.h"
#include "ThreadedSim.h"


#define DELTAX	.1
#define PI 3.14159265358979323846264

GLfloat light_ambient		[] = { 0.5,	0.5,	0.5,	0.1	} ;
GLfloat light_diffuse		[] = { 1.0,		1.0,	1.0,	1.0		} ;
GLfloat light_specular		[] = { 0.8,		0.8,	0.8,	1.0		} ;

GLfloat light1_position     [] = { 15.0,	5.0,	10.0,	1.0		} ;
GLfloat light2_ambient      [] = { 1.0,		1.0,	1.0,	1.0 	} ;
GLfloat light2_diffuse		[] = { 0.0,		0.0,	0.0,	1.0		} ;
GLfloat light2_position		[] = { -12.0,	-20.0,	-18.0,	1.0		} ;

GLfloat none_mat_specular	[] = { 0.0,		0.0,	0.0,	0.0		} ;

GLsizei windoww,windowh;

float rotateX=20, rotateY=20;
float angle=-10.0;

extern int show_box;
extern int show_spheres;

int dX=100,dY=10,dZ=100;
float moveX=0.0,moveY=0.0;
int rotate_down=0;
int hight_down=0;
int move_down=0;
static int sizex,sizey;

extern ThreadedSim * tsim;
//extern int numBalls;

void DrawBoundingBox()
{
	glDisable	( GL_LIGHTING   );
	glColor3f(1,1,1);
	glLineWidth(0.1);
	glPushMatrix();
	//glScalef(0.8,0.8,0.8);
	glBegin(GL_LINES);
		glVertex3f(-10.0,-10,-10.0);
		glVertex3f(10.0,-10,-10.0);
		glVertex3f(-10.0,-10,-10.0);
		glVertex3f(-10.0,10,-10.0);
		glVertex3f(-10.0,-10,-10.0);
		glVertex3f(-10.0,-10,10.0);
		glVertex3f(10.0,10,10.0);
		glVertex3f(-10.0,10,10.0);
		glVertex3f(-10.0,10,10.0);
		glVertex3f(-10.0,-10,10.0);
		glVertex3f(-10.0,10,10.0);
		glVertex3f(-10.0,10,-10.0);
		glVertex3f(10.0,10,10.0);
		glVertex3f(10.0,-10,10.0);
		glVertex3f(10.0,-10,10.0);
		glVertex3f(10.0,-10,-10.0);
		glVertex3f(10.0,-10,10.0);
		glVertex3f(-10.0,-10,10.0);
		glVertex3f(10.0,10,10.0);
		glVertex3f(10.0,10,-10.0);
		glVertex3f(10.0,10,-10.0);
		glVertex3f(-10.0,10,-10.0);
		glVertex3f(10.0,10,-10.0);
		glVertex3f(10.0,-10,-10.0);
	glEnd();
	glPopMatrix();
	glEnable(GL_LIGHTING);
}


void CALLBACK rotateview ( int x, int y)
{
	static int oldx,oldy;
    if (rotate_down){
		rotateY += x - oldx;
		rotateX += y - oldy;
		rotateX=(int)(rotateX+360)%360;
		rotateY=(int)(rotateY+360)%360;
        oldx=x;
        oldy=y;
    }
    else{
    	rotate_down=1;
        oldx=x;
        oldy=y;
    }
}

void CALLBACK pressbutton ()
{
	rotate_down=0;
}

void CALLBACK hightview ( int x,int y )
{
	static int oldh;
    if (hight_down){
		angle -= (y - oldh)*0.5;
        oldh=y;
    }
    else{
    	hight_down=1;
        oldh=y;
    }
}

void CALLBACK moveview(int x, int y)
{
	static int oldx,oldy;
    if (move_down){
		moveX -= float(x - oldx)/5.0;
		moveY += float(y - oldy)/5.0;
        oldx=x;
        oldy=y;
    }
    else{
    	move_down=1;
        oldx=x;
        oldy=y;
    }
}

void CALLBACK pressbutton2 ()
{
	hight_down=0;
}

void CALLBACK pressbutton3 ()
{
	move_down=0;
}

void CALLBACK Animate(int i)
{
	glEnable(GL_DEPTH_TEST);
	bool oldvalue;
    glClear			( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT  );
	glLoadIdentity  ( );
	glTranslatef	( moveX, moveY, -20.0f+angle );
	glRotatef		( rotateX, 1.0f, 0.0f, 0.0f );
//	rotateY +=rotation_per_frame;
	glRotatef		( rotateY, 0.0f, 1.0f, 0.0f );
	glLightfv	 ( GL_LIGHT0,	GL_AMBIENT,			light_ambient ) ;
	glLightfv	 ( GL_LIGHT0,	GL_DIFFUSE,			light_diffuse ) ;
	glLightfv	 ( GL_LIGHT0,	GL_SPECULAR,		light_specular ) ;
	glLightfv	 ( GL_LIGHT0,	GL_POSITION,		light1_position ) ;
//	glLightf	 ( GL_LIGHT0, GL_LINEAR_ATTENUATION , 0.01);
	glEnable	 ( GL_LIGHT0 );
	glLightfv	 ( GL_LIGHT1,	GL_AMBIENT,			light_ambient ) ;
	glLightfv	 ( GL_LIGHT1,	GL_DIFFUSE,			light_diffuse ) ;
	glLightfv	 ( GL_LIGHT1,	GL_SPECULAR,		light_specular ) ;
	glLightfv	 ( GL_LIGHT1,	GL_POSITION,		light2_position ) ;
	glEnable	 ( GL_LIGHT1 );
	glEnable	 (GL_LIGHTING);

	if(show_spheres)
		tsim->Show();

	if(show_box)
		DrawBoundingBox();
}

void CALLBACK ResizeWindow(GLsizei w, GLsizei h)
{
    windowh=h = (h == 0) ? 1 : h;
	windoww=w = (w == 0) ? 1 : w;
	glViewport		( 0, 0, w, h );
    glMatrixMode	( GL_PROJECTION );
    glLoadIdentity	( );
	gluPerspective( 40.0, (GLfloat) w/(GLfloat) h, 1.0, 400.0 ) ;
 	glMatrixMode	( GL_MODELVIEW );
    sizex=w;
    sizey=h;
}

void main3D( void )
{
	glClearColor( 1.0,1.0,1.0, 0.0 );
	glClearDepth( 1.0f			); 

	glShadeModel	( GL_SMOOTH );
	glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    glEnable        ( GL_COLOR_MATERIAL );
    return;
}
