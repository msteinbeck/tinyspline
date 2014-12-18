#include "tinyspline.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h> 
#include "debugging.h"

tsBSpline spline;
GLUnurbsObj *theNurb;
float u = 0;

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
}

void tear_down()
{
    ts_bspline_free(&spline);
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
            spline.n_knots, 
            &spline.knots[0], 
            spline.dim, 
            spline.ctrlp, 
            spline.order, 
            GL_MAP1_VERTEX_3
        );
    gluEndCurve(theNurb);
    
    // draw split
    glColor3f(0.0, 0.0, 1.0);
    glPointSize(5.0);
    tsBSplineSequence seq;
    int ret = ts_bspline_split(&spline, u, &seq);
    if (ret >= 0) {
        gluBeginCurve(theNurb);
            gluNurbsCurve(
                theNurb, 
                seq.bsplines[0].n_knots, 
                &seq.bsplines[0].knots[0], 
                seq.bsplines[0].dim, 
                seq.bsplines[0].ctrlp, 
                seq.bsplines[0].order, 
                GL_MAP1_VERTEX_3
            );
        gluEndCurve(theNurb);
        
        if (ret == 0) {
            glColor3f(0.0, 1.0, 0.0);
            gluBeginCurve(theNurb);
                gluNurbsCurve(
                    theNurb, 
                    seq.bsplines[1].n_knots, 
                    &seq.bsplines[1].knots[0], 
                    seq.bsplines[1].dim, 
                    seq.bsplines[1].ctrlp, 
                    seq.bsplines[1].order, 
                    GL_MAP1_VERTEX_3
                );
            gluEndCurve(theNurb);
        }
    }
    
    u += 0.001;
    if (u > 1.f) {
        u = 0.f;
    }
    
    // flush output
    glFlush();
    glutPostRedisplay();
}


/********************************************************
*                                                       *
* Framework                                             *
*                                                       *
********************************************************/
void glutTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(1, glutTimer, 1);
} 

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
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
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
