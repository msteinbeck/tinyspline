#include "tinyspline.h"

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h> 
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h> 

tsBSpline spline;
GLUnurbsObj *theNurb;
tsReal b = 1.f;

/********************************************************
*                                                       *
* Modify these lines for experimenting.                 *
*                                                       *
********************************************************/
void setup()
{
	ts_bspline_new(
		3,      /* number of control points */
		3,      /* dimension of each point */
		2,      /* degree of spline */
		TS_CLAMPED, /* used to hit first and last control point */
		&spline /* the spline to setup */
	);
	
	/* Setup control points. */
	spline.ctrlp[0] = -1.0f;
	spline.ctrlp[1] = 1.0f;
	spline.ctrlp[2] = 0.0f;
	spline.ctrlp[3] = 1.0f;
	spline.ctrlp[4] = 1.0f;
	spline.ctrlp[5] = 0.0f;
	spline.ctrlp[6] = 1.0f;
	spline.ctrlp[7] = -1.0f;
	spline.ctrlp[8] = 0.0f;
}

void tear_down()
{
	ts_bspline_free(&spline);
}

void display(void)
{
	tsBSpline buckled;
	size_t i;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ts_bspline_buckle(&spline, b, &buckled);
	
	/* draw spline */
	glColor3f(1.0, 1.0, 1.0);
	glLineWidth(3);
	gluBeginCurve(theNurb);
		gluNurbsCurve(
			theNurb, 
			(GLint)buckled.n_knots,
			buckled.knots,
			(GLint)buckled.dim,
			buckled.ctrlp,
			(GLint)buckled.order,
			GL_MAP1_VERTEX_3
		);
	gluEndCurve(theNurb);

	/* draw control points */
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(5.0);
	glBegin(GL_POINTS);
	  for (i = 0; i < spline.n_ctrlp; i++) 
		 glVertex3fv(&spline.ctrlp[i * spline.dim]);
	glEnd();
	
	glutSwapBuffers();
	glutPostRedisplay();

	b -= 0.001f;
	if (b < 0.f) {
		b = 1.f;
	}
}




/********************************************************
*                                                       *
* Framework                                             *
*                                                       *
********************************************************/
void nurbsError(GLenum errorCode)
{
   const GLubyte *estring;

   estring = gluErrorString(errorCode);
   fprintf (stderr, "Nurbs Error: %s\n", estring);
   exit (0);
}
   
void init(void)
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	theNurb = gluNewNurbsRenderer();
	gluNurbsProperty (theNurb, GLU_SAMPLING_TOLERANCE, 10.0);
	gluNurbsCallback(theNurb, GLU_ERROR, (GLvoid (*)()) nurbsError);
	setup();
}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective (45.0, (GLdouble)w/(GLdouble)h, 3.0, 8.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef (0.0, 0.0, -5.0);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (500, 500);
	glutInitWindowPosition (100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMainLoop();
	tear_down();
	return 0; 
}
