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
tsReal u = 0.f;

/********************************************************
*                                                       *
* Modify these lines for experimenting.                 *
*                                                       *
********************************************************/
void setup()
{
	const tsReal w = (tsReal)(sqrt(2.f) / 2.f);
	ts_bspline_new(9, 4, 2, TS_CLAMPED ,&spline);
	
	spline.ctrlp[0] = 1.f;
	spline.ctrlp[1] = 0.f;
	spline.ctrlp[2] = 0.f;
	spline.ctrlp[3] = 1.f;
	
	spline.ctrlp[4] = w;
	spline.ctrlp[5] = w;
	spline.ctrlp[6] = 0.f;
	spline.ctrlp[7] = w;
	
	spline.ctrlp[8] = 0.f;
	spline.ctrlp[9] = 1.f;
	spline.ctrlp[10] = 0.f;
	spline.ctrlp[11] = 1.f;
	
	spline.ctrlp[12]  = -w;
	spline.ctrlp[13] = w;
	spline.ctrlp[14] = 0.f;
	spline.ctrlp[15] = w;
	
	spline.ctrlp[16] = -1.f;
	spline.ctrlp[17] = 0.f;
	spline.ctrlp[18] = 0.f;
	spline.ctrlp[19] = 1.f;
	
	spline.ctrlp[20] = -w;
	spline.ctrlp[21] = -w;
	spline.ctrlp[22] = 0.f;
	spline.ctrlp[23] = w;
	
	spline.ctrlp[24] = 0.f;
	spline.ctrlp[25] = -1.f;
	spline.ctrlp[26] = 0.f;
	spline.ctrlp[27] = 1.f;
	
	spline.ctrlp[28] = w;
	spline.ctrlp[29] = -w;
	spline.ctrlp[30] = 0.f;
	spline.ctrlp[31] = w;
	
	spline.ctrlp[32] = 1.f;
	spline.ctrlp[33] = 0.f;
	spline.ctrlp[34] = 0.f;
	spline.ctrlp[35] = 1.f;
	
	spline.knots[0] = 0.f;
	spline.knots[1] = 0.f;
	spline.knots[2] = 0.f;
	spline.knots[3] = 1.f/4.f;
	spline.knots[4] = 1.f/4.f;
	spline.knots[5] = 2.f/4.f;
	spline.knots[6] = 2.f/4.f;
	spline.knots[7] = 3.f/4.f;
	spline.knots[8] = 3.f/4.f;
	spline.knots[9] = 1.f;
	spline.knots[10] = 1.f;
	spline.knots[11] = 1.f;
}

void tear_down()
{
	ts_bspline_free(&spline);
}

void display(void)
{
	tsDeBoorNet net;
	size_t i;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	/* draw spline */
	glColor3f(1.0, 1.0, 1.0);
	glLineWidth(3);
	gluBeginCurve(theNurb);
		gluNurbsCurve(
			theNurb, 
			(GLint)spline.n_knots,
			spline.knots, 
			(GLint)spline.dim,
			spline.ctrlp, 
			(GLint)spline.order,
			GL_MAP1_VERTEX_4
		);
	gluEndCurve(theNurb);

	/* draw control points */
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(5.0);
	glBegin(GL_POINTS);
	  for (i = 0; i < spline.n_ctrlp; i++) 
		 glVertex3fv(&spline.ctrlp[i * spline.dim]);
	glEnd();
	
	/* draw evaluation */
	glColor3f(0.0, 0.0, 1.0);
	glPointSize(5.0);
	ts_bspline_evaluate(&spline, u, &net);
	glBegin(GL_POINTS);
		glVertex4fv(net.result);
	glEnd();
	ts_deboornet_free(&net);
	
	u += 0.001f;
	if (u > 1.f) {
		u = 0.f;
	}
	
	glutSwapBuffers();
	glutPostRedisplay();
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
