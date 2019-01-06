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
	tsReal *ctrlp, *knots;
	const tsReal w = (tsReal) (sqrt(2) / 2.f);

	ts_bspline_new(9, 4, 2, TS_CLAMPED ,&spline, NULL);

	/* set control points */
	ts_bspline_control_points(&spline, &ctrlp, NULL);
	ctrlp[0] = 1.f;
	ctrlp[1] = 0.f;
	ctrlp[2] = 0.f;
	ctrlp[3] = 1.f;

	ctrlp[4] = w;
	ctrlp[5] = w;
	ctrlp[6] = 0.f;
	ctrlp[7] = w;

	ctrlp[8] = 0.f;
	ctrlp[9] = 1.f;
	ctrlp[10] = 0.f;
	ctrlp[11] = 1.f;

	ctrlp[12]  = -w;
	ctrlp[13] = w;
	ctrlp[14] = 0.f;
	ctrlp[15] = w;

	ctrlp[16] = -1.f;
	ctrlp[17] = 0.f;
	ctrlp[18] = 0.f;
	ctrlp[19] = 1.f;

	ctrlp[20] = -w;
	ctrlp[21] = -w;
	ctrlp[22] = 0.f;
	ctrlp[23] = w;

	ctrlp[24] = 0.f;
	ctrlp[25] = -1.f;
	ctrlp[26] = 0.f;
	ctrlp[27] = 1.f;

	ctrlp[28] = w;
	ctrlp[29] = -w;
	ctrlp[30] = 0.f;
	ctrlp[31] = w;

	ctrlp[32] = 1.f;
	ctrlp[33] = 0.f;
	ctrlp[34] = 0.f;
	ctrlp[35] = 1.f;
	ts_bspline_set_control_points(&spline, ctrlp, NULL);
	free(ctrlp);
	
	/* set knots */
	ts_bspline_knots(&spline, &knots, NULL);
	knots[0] = 0.f;
	knots[1] = 0.f;
	knots[2] = 0.f;
	knots[3] = 1.f/4.f;
	knots[4] = 1.f/4.f;
	knots[5] = 2.f/4.f;
	knots[6] = 2.f/4.f;
	knots[7] = 3.f/4.f;
	knots[8] = 3.f/4.f;
	knots[9] = 1.f;
	knots[10] = 1.f;
	knots[11] = 1.f;
	ts_bspline_set_knots(&spline, knots, NULL);
	free(knots);
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
			GL_MAP1_VERTEX_4
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
	ts_bspline_eval(&spline, u, &net, NULL);
	ts_deboornet_result(&net, &result, NULL);
	
	/* draw evaluation */
	glColor3f(0.0, 0.0, 1.0);
	glPointSize(5.0);
	glBegin(GL_POINTS);
		glVertex4fv(result);
	glEnd();
	
	ts_deboornet_free(&net);
	free(ctrlp);
	free(knots);
	free(result);
	
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
