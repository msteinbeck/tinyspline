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
#include <string.h>
#include <tinyspline.h>

tsBSpline spline;
tsReal *ctrlp, *knots;
GLUnurbsObj *theNurb;
size_t i; /* loop counter */
tsReal t = 0.f;
/* A, C, D, and E are convenience variables and simply wrap the access
to the control points of `spline`. B is required to store the original
second control point of `spline` which gets replaced with D and E.
v and w are the vectors AB and CB, respectively. */
tsReal *A, *C, *D, *E;
tsReal B[3], v[3], w[3];

/********************************************************
*                                                       *
* Modify these lines for experimenting.                 *
*                                                       *
********************************************************/
void setup()
{
	size_t k;
	tsReal mid;

	ts_bspline_new(
		3,      /* number of control points */
		3,      /* dimension of each point */
		2,      /* degree of spline */
		TS_CLAMPED, /* used to hit first and last control point */
		&spline, /* the spline to setup */
		NULL);
	
	/* Setup control points. */
	ts_bspline_control_points(&spline, &ctrlp, NULL);
	ctrlp[0] = -1.0f;
	ctrlp[1] =  1.0f;
	ctrlp[2] =  0.0f;
	ctrlp[3] =  1.0f;
	ctrlp[4] =  1.0f;
	ctrlp[5] =  0.0f;
	ctrlp[6] =  1.0f;
	ctrlp[7] = -1.0f;
	ctrlp[8] =  0.0f;
	ts_bspline_set_control_points(&spline, ctrlp, NULL);
	free(ctrlp);

	for (i = 0; i < 3; i++)
		B[i] = ctrlp[i+3];

	ts_bspline_knots(&spline, &knots, NULL);
	mid = (knots[ts_bspline_num_knots(&spline)- 1] - knots[0]) /2;
	free(knots);

	ts_bspline_insert_knot(&spline, mid, 1, &spline, &k, NULL);
	ts_bspline_control_points(&spline, &ctrlp, NULL);
	ts_bspline_knots(&spline, &knots, NULL);

	A = ctrlp;
	D = ctrlp + 3;
	E = ctrlp + 6;
	C = ctrlp + 9;

	for (i = 0; i < 3; i++) {
		v[i] = B[i] - A[i];
		w[i] = B[i] - C[i];
	}
}

void tear_down()
{
	ts_bspline_free(&spline);
	free(ctrlp);
	free(knots);
}

void displayText(float x, float y, float r, float g, float b, const char *string)
{
	size_t str_sz = strlen(string);
	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	for(i = 0; i < str_sz; i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}

void display(void)
{
	char buffer[256];
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* keep in mind that `D` and `E` are simply pointers to the
	corresponding control points of `spline`. */
	for (i = 0; i < 3; i++) {
		D[i] = A[i] + t*v[i];
		E[i] = C[i] + t*w[i];
	}
	ts_bspline_set_control_points(&spline, ctrlp, NULL);

	/* draw spline */
	glColor3f(1.0, 1.0, 1.0);
	glLineWidth(3);
	gluBeginCurve(theNurb);
		gluNurbsCurve(
			theNurb, 
			(GLint)ts_bspline_num_knots(&spline),
			knots,
			(GLint)ts_bspline_dimension(&spline),
			ctrlp,
			(GLint)ts_bspline_order(&spline),
			GL_MAP1_VERTEX_3
		);
	gluEndCurve(theNurb);

	/* draw control points */
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(5.0);
	glBegin(GL_POINTS);
	  for (i = 0; i < ts_bspline_num_control_points(&spline); i++)
		 glVertex3fv(&ctrlp[i * ts_bspline_dimension(&spline)]);
	glEnd();

	/* draw B */
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_POINTS);
		glVertex3fv(B);
	glEnd();

	/* display t */
	sprintf(buffer, "t: %.2f", t);
	displayText(-.2f, 1.2f, 0.0, 1.0, 0.0, buffer);

	glutSwapBuffers();
	glutPostRedisplay();

	t += 0.001f;
	if (t > 1.f) {
		t = 0.f;
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
   glTranslatef (0.0f, 0.0f, -5.0f);
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
