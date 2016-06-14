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

tsBSpline spline, derivative;
GLUnurbsObj *theNurb;
float u = 0.f;

/********************************************************
*                                                       *
* Modify these lines for experimenting.                 *
*                                                       *
********************************************************/
void setup()
{
    ts_bspline_new(
        3,      // degree of spline
        3,      // dimension of each point
        7,      // number of control points
        TS_CLAMPED,// used to hit first and last control point
        &spline // the spline to setup
    );
    
    // Setup control points.
    spline.ctrlp[0] = -1.75;
    spline.ctrlp[1] = -1.0;
    spline.ctrlp[2] = 0.0;
    spline.ctrlp[3] = -1.5;
    spline.ctrlp[4] = -0.5;
    spline.ctrlp[5] = 0.0;
    spline.ctrlp[6] = -1.5;
    spline.ctrlp[7] = 0.0;
    spline.ctrlp[8] = 0.0;
    spline.ctrlp[9] = -1.25;
    spline.ctrlp[10] = 0.5;
    spline.ctrlp[11] = 0.0;
    spline.ctrlp[12] = -0.75;
    spline.ctrlp[13] = 0.75;
    spline.ctrlp[14] = 0.0;
    spline.ctrlp[15] = 0.0;
    spline.ctrlp[16] = 0.5;
    spline.ctrlp[17] = 0.0;
    spline.ctrlp[18] = 0.5;
    spline.ctrlp[19] = 0.0;
    spline.ctrlp[20] = 0.0;

    ts_bspline_derive(&spline, &derivative);
}

void tear_down()
{
    ts_bspline_free(&spline);
    ts_bspline_free(&derivative);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // draw spline
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

    // draw control points
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(5.0);
    size_t i;
    glBegin(GL_POINTS);
      for (i = 0; i < spline.n_ctrlp; i++) 
         glVertex3fv(&spline.ctrlp[i * spline.dim]);
    glEnd();
    
    // draw derivative
    glColor3f(0.0, 0.0, 1.0);
    glPointSize(5.0);
    tsDeBoorNet net1, net2, net3;
    ts_bspline_evaluate(&spline, u, &net1);
    ts_bspline_evaluate(&derivative, u, &net2);
    ts_bspline_evaluate(&derivative, u, &net3);
    for (i = 0; i < net2.dim; i++) {
        // subdivided by 6 just to avoid the tangent to exit from the window
        net2.result[i] = net1.result[i] + net2.result[i] / 6.f;
        net3.result[i] = net1.result[i] - net3.result[i] / 6.f;
    }
    glBegin(GL_LINES);
        glVertex3fv(net3.result);
        glVertex3fv(net2.result);
    glEnd();
    ts_deboornet_free(&net1);
    ts_deboornet_free(&net2);
    ts_deboornet_free(&net3);

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
