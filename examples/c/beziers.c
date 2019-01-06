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

tsBSpline spline;
GLUnurbsObj *theNurb;
int drawBeziers = 0; /* 0 - bspline, 1 - beziers */

/********************************************************
*                                                       *
* Modify these lines for experimenting.                 *
*                                                       *
********************************************************/
void setup()
{
	tsReal *ctrlp;

	ts_bspline_new(
		7,      /* number of control points */
		3,      /* dimension of each point */
		3,      /* degree of spline */
		TS_CLAMPED, /* used to hit first and last control point */
		&spline, /* the spline to setup */
		NULL);
	
	/* Setup control points. */
	ts_bspline_control_points(&spline, &ctrlp, NULL);
	ctrlp[0]  = -1.75f;
	ctrlp[1]  = -1.0f;
	ctrlp[2]  =  0.0f;
	ctrlp[3]  = -1.5f;
	ctrlp[4]  = -0.5f;
	ctrlp[5]  =  0.0f;
	ctrlp[6]  = -1.5f;
	ctrlp[7]  =  0.0f;
	ctrlp[8]  =  0.0f;
	ctrlp[9]  = -1.25f;
	ctrlp[10] =  0.5f;
	ctrlp[11] =  0.0f;
	ctrlp[12] = -0.75f;
	ctrlp[13] =  0.75f;
	ctrlp[14] =  0.0f;
	ctrlp[15] =  0.0f;
	ctrlp[16] =  0.5f;
	ctrlp[17] =  0.0f;
	ctrlp[18] =  0.5f;
	ctrlp[19] =  0.0f;
	ctrlp[20] =  0.0f;
	ts_bspline_set_control_points(&spline, ctrlp, NULL);
	free(ctrlp);
}

void tear_down()
{
	ts_bspline_free(&spline);
}

void display(void)
{
	size_t i;
	tsBSpline draw;
	tsReal *ctrlp_spline, *ctrlp_draw;
	tsReal *knots_draw;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (drawBeziers)
		ts_bspline_to_beziers(&spline, &draw, NULL);
	else
		ts_bspline_copy(&spline, &draw, NULL);

	/* draw 'draw' */
	ts_bspline_control_points(&draw, &ctrlp_draw, NULL);
	ts_bspline_knots(&draw, &knots_draw, NULL);
	glColor3f(1.0, 1.0, 1.0);
	glLineWidth(3);
	gluBeginCurve(theNurb);
		gluNurbsCurve(
			theNurb,
			(GLint)ts_bspline_num_knots(&draw),
			knots_draw,
			(GLint)ts_bspline_dimension(&draw),
			ctrlp_draw,
			(GLint)ts_bspline_order(&draw),
			GL_MAP1_VERTEX_3
		);
	gluEndCurve(theNurb);
	ts_bspline_free(&draw);

	/* draw control points */
	ts_bspline_control_points(&spline, &ctrlp_spline, NULL);
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(5.0);
	glBegin(GL_POINTS);
	  for (i = 0; i < ts_bspline_num_control_points(&spline); i++)
		 glVertex3fv(&ctrlp_spline[i * ts_bspline_dimension(&spline)]);
	glEnd();

	ts_bspline_free(&draw);
	free(ctrlp_spline);
	free(ctrlp_draw);
	free(knots_draw);
	
	glutSwapBuffers();
	glutPostRedisplay();
}

void keyPressed (unsigned char key, int x, int y) {
	if (key == ' ') {
		if (drawBeziers) {
			printf("draw bspline\n");
			drawBeziers = 0;
		} else {
			printf("draw beziers\n");
			drawBeziers = 1;
		}
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
	glutKeyboardFunc(keyPressed);
	glutMainLoop();
	tear_down();
	return 0; 
}
