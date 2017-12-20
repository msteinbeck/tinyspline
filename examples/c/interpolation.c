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

#include <stdio.h>
#include <debugging.h>

tsBSpline spline;
GLUnurbsObj *theNurb;

/********************************************************
*                                                       *
* Modify these lines for experimenting.                 *
*                                                       *
********************************************************/
void setup()
{
	tsReal points[15];
	points[0] = 1;
	points[1] = -1;
	points[2] = 0;
	points[3] = -1;
	points[4] = 2;
	points[5] = 0;
	points[6] = 1;
	points[7] = 4;
	points[8] = 0;
	points[9] = 4;
	points[10] = 3;
	points[11] = 0;
	points[12] = 7;
	points[13] = 5;
	points[14] = 0;

	ts_bspline_interpolate_cubic(points, 5, 3, &spline);
	ts_bspline_print(&spline);
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
	
	/* draw evaluation */
	glColor3f(0.0, 0.0, 1.0);
	glPointSize(5.0);
	ts_bspline_evaluate(&spline, 0.5f, &net);
	glBegin(GL_POINTS);
		glVertex3fv(net.result);
	glEnd();
	ts_deboornet_free(&net);
	
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
   gluPerspective (80.0, (GLdouble)w/(GLdouble)h, 3.0, 8.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef (-2.5, -1.0, -7.0);
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
