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
GLUnurbsObj *theNurb;
size_t i; /* loop counter */
/* the original second point of `spline` that is
weight with `w` in every call of ::display */
tsReal B[4] = {1.0f, 1.0f, 0.0f, 1.0f};
tsReal w = 0.001f;

/********************************************************
*                                                       *
* Modify these lines for experimenting.                 *
*                                                       *
********************************************************/
void setup()
{
	tsReal *ctrlp;

	ts_bspline_new(
		3,      /* number of control points */
		4,      /* dimension of each point */
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

	/* ignore second control point here since it is
	updated in ::display anyway */

	ctrlp[8]  =  1.0f;
	ctrlp[9]  = -1.0f;
	ctrlp[10] =  0.0f;
	ctrlp[11] =  1.0f;
	ts_bspline_set_control_points(&spline, ctrlp, NULL);
	free(ctrlp);
}

void tear_down()
{
	ts_bspline_free(&spline);
}

void displayText( float x, float y, float r, float g, float b, const char *string )
{
	size_t str_sz = strlen( string );
	glColor3f( r, g, b );
	glRasterPos2f( x, y );
	for(i = 0; i < str_sz; i++) {
		glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, string[i] );
	}
}

void display(void)
{
	tsReal *ctrlp, *knots;
	ts_bspline_control_points(&spline, &ctrlp, NULL);
	ts_bspline_knots(&spline, &knots, NULL);

	char buffer[256];
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* weight the first three components with `w` and store `w`
	in the forth component of the control point---that's how
	homogeneous coordinates work */
	ctrlp[4] = B[0] * w;
	ctrlp[5] = B[1] * w;
	ctrlp[6] = B[2] * w;
	ctrlp[7] = w;
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
			GL_MAP1_VERTEX_4
		);
	gluEndCurve(theNurb);

	/* draw control points */
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(5.0);
	glBegin(GL_POINTS);
		 glVertex3fv(ctrlp);
		 glVertex3fv(B);
		 glVertex3fv(ctrlp + 8);
	glEnd();

	/* display w */
	sprintf( buffer, "w: %.2f", w );
	displayText( -.2f, 1.2f, 0.0, 1.0, 0.0, buffer );
	
	glutSwapBuffers();
	glutPostRedisplay();
	
	free(ctrlp);
	free(knots);

	w *= 1.05f; /* non-linear update of `w` */
	if (w > 100.f) {
		w = 0.001f;
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
