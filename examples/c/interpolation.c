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
	points[0]  =  1;
	points[1]  = -1;
	points[2]  =  0;
	points[3]  = -1;
	points[4]  =  2;
	points[5]  =  0;
	points[6]  =  1;
	points[7]  =  4;
	points[8]  =  0;
	points[9]  =  4;
	points[10] =  3;
	points[11] =  0;
	points[12] =  7;
	points[13] =  5;
	points[14] =  0;

	ts_bspline_interpolate_cubic(points, 5, 3, &spline, NULL);
	ts_bspline_print(&spline);
}

void tear_down()
{
	ts_bspline_free(&spline);
}

void display(void)
{
	size_t i;
	tsDeBoorNet net;
	tsReal *ctrlp;
	tsReal *knots;
	tsReal *result;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	/* draw spline */
	ts_bspline_control_points(&spline, &ctrlp, NULL);
	ts_bspline_knots(&spline, &knots, NULL);
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

	/* eval spline */
	ts_bspline_eval(&spline, 0.5f, &net, NULL);
	ts_deboornet_result(&net, &result, NULL);
	
	/* draw evaluation */
	glColor3f(0.0, 0.0, 1.0);
	glPointSize(5.0);
	glBegin(GL_POINTS);
		glVertex3fv(result);
	glEnd();
	ts_deboornet_free(&net);

	ts_deboornet_free(&net);
	free(ctrlp);
	free(knots);
	free(result);
	
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
   glTranslatef (-2.5f, -1.0f, -7.0f);
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
