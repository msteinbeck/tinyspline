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
#include <assert.h>

/*
   Example shows derivative on a spline created by
   ts_bspline_interpolate_cubic
   use ts_bspline_clean on the created spline 
   before calling ts_bspline_derive
*/


/*
  clean a spline created by ts_bspline_interpolate_cubic
  so that ts_bspline_derive works correctly
  The spline is cleaned in place
*/
static tsError ts_bspline_clean(tsBSpline * spline)
{
   const size_t order = spline->order;
   const size_t dim = spline->dim;
   const tsReal min_useful_distance = (tsReal)1.e-6;// ? for now

   size_t knot_idx = 0U;
   // n.b that number of knots and control points in the spline_copy may change dynamically in the loop
   while(knot_idx < spline->n_knots){
      // count the multiplicity of the knot
      size_t multiplicity = 1U;
      const tsReal cur_knot_value = spline->knots[knot_idx];
      while ( (knot_idx < (spline->n_knots-1)) && ( spline->knots[knot_idx +1] == cur_knot_value) ){
         ++knot_idx;
         ++multiplicity;
      }
      if ( multiplicity == order){
         const size_t idxa = (knot_idx - multiplicity) * dim;
         const size_t idxb = idxa + dim;
         // get and compare the control points
         const tsReal* const ctrl_pointsa = &spline->ctrlp[idxa];
         const tsReal* const ctrl_pointsb = &spline->ctrlp[idxb];
     
         if (ts_ctrlp_dist2(ctrl_pointsa,ctrl_pointsb, dim) < min_useful_distance){

            // create and init a temporary for the modified spline
            tsBSpline mod_spline;
            
            mod_spline.deg = spline->deg;
            mod_spline.order = spline->order;
            mod_spline.dim = spline->dim;
            mod_spline.n_ctrlp = spline->n_ctrlp-1U;
            mod_spline.n_knots = spline->n_knots-1U;

            const size_t n_mod_ctrlp_reals = mod_spline.n_ctrlp * mod_spline.dim;
            const size_t n_mod_raw_bytes_for_real_array = sizeof(tsReal) *  (mod_spline.n_knots + n_mod_ctrlp_reals);
            mod_spline.ctrlp = (tsReal*) malloc(n_mod_raw_bytes_for_real_array);
            if ( mod_spline.ctrlp == NULL){
               return TS_MALLOC;
            }
            // knots array is at an offset in the ctrlpoints array
            mod_spline.knots = mod_spline.ctrlp + n_mod_ctrlp_reals;
            
            // copy control points prior to and including first control point corresponding to current knot
            const tsReal* ctrlp_src = spline->ctrlp;
            tsReal* ctrlp_dest = mod_spline.ctrlp;
            size_t reals_copied = 0U;
            while ( ctrlp_src != ctrl_pointsb){
               *ctrlp_dest++ = *ctrlp_src++;
                ++reals_copied;
            }
            // skip copying 2nd control point corresponding to current knot
            ctrlp_src +=  mod_spline.dim;
            // copy the rest of the control points over
            while ( reals_copied < n_mod_ctrlp_reals ){
                *ctrlp_dest++ = *ctrlp_src++;
                ++reals_copied;
            }
            // copy all knots before current
            const tsReal* const knots_src = spline->knots;
            tsReal* const knots_dest = mod_spline.knots;
            size_t idx = 0U;
            while ( idx < knot_idx){
               knots_dest[idx] = knots_src[idx];
               ++idx;
            }
            // copy all knots after current
            while (idx < mod_spline.n_knots){
               knots_dest[idx] = knots_src[idx + 1];
               ++idx;
            } 
            // clear the original spline_copy and replace the contents with the new one  
            ts_bspline_free(spline);
            ts_bspline_move(&mod_spline,spline);
         }
      }else{
         ++knot_idx;
      }
   }
   return TS_SUCCESS;
}

/********************************************************
*                                                       *
* Modify these lines for experimenting.                 *
*                                                       *
********************************************************/

static tsBSpline spline_orig;
static tsBSpline spline_copy;
static tsBSpline deriv;

static GLUnurbsObj *theNurb;
void setup()
{
   printf ("interpolated spline derivative test with control point cleaning\n\n");

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

	ts_bspline_interpolate_cubic(points, 5, 3, &spline_orig);
   ts_bspline_print(&spline_orig);
   assert(ts_bspline_copy(&spline_orig,&spline_copy)== TS_SUCCESS);
   assert(ts_bspline_clean(&spline_copy) == TS_SUCCESS);
   assert(ts_bspline_derive(&spline_copy,&deriv) == TS_SUCCESS);
}

void tear_down()
{
	ts_bspline_free(&spline_copy);
   ts_bspline_free(&spline_orig);
   ts_bspline_free(&deriv);
}

// function to show the two splines in varying colours, where each colour is a array of dim 3
static void draw_spline(tsBSpline * spline,GLfloat* spline_colour,GLfloat* ctrlp_colour,GLfloat* eval_colour)
{
   tsDeBoorNet net;
	glLineWidth(3);
   glColor3f(spline_colour[0],spline_colour[1],spline_colour[2]);
	gluBeginCurve(theNurb);
		gluNurbsCurve(
			theNurb, 
			(GLint)spline->n_knots,
			spline->knots, 
			(GLint)spline->dim,
			spline->ctrlp, 
			(GLint)spline->order,
			GL_MAP1_VERTEX_3
		);
	gluEndCurve(theNurb);

   glColor3f(ctrlp_colour[0],ctrlp_colour[1],ctrlp_colour[2]);
	glPointSize(5.0);
	glBegin(GL_POINTS);
     size_t i;
	  for (i = 0; i < spline->n_ctrlp; i++) 
		 glVertex3fv(&spline->ctrlp[i * spline->dim]);
	glEnd();
 
	/* draw evaluation */
	glColor3f(eval_colour[0],eval_colour[1],eval_colour[2]);
	glPointSize(5.0);
	ts_bspline_evaluate(spline, 0.5f, &net);
	glBegin(GL_POINTS);
		glVertex3fv(net.result);
	glEnd();
   ts_deboornet_free(&net);
}

/*
   draw the tangent of the spline at u in colour
   derivative has been evaluated by calling ts_bspline_derive on the spline
*/
static void draw_tangent(tsBSpline * spline, tsBSpline * derivative, tsReal u, GLfloat* colour)
{
   tsDeBoorNet net1, net2, net3;
   	/* draw derivative */
	glColor3f(colour[0],colour[1],colour[2]);
	glPointSize(5.0);
	ts_bspline_evaluate(spline, u, &net1);
	ts_bspline_evaluate(derivative, u, &net2);
	ts_bspline_evaluate(derivative, u, &net3);
   size_t i;
	for (i = 0; i < net2.dim; i++) {
		/* subdivided by 6 just to avoid the tangent to exit from the window */
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
}

static GLfloat red[]= { 1.0,0.0,0.0};
static GLfloat green[]= { 0,1.0,0.0};
static GLfloat blue[]= { 0.0,0.0,1.0};
static GLfloat grey[]= { 0.5,0.5,0.5};
static GLfloat yellow[]= { 1.0,1.0,0.0};
static GLfloat orange[]= { 1.0,0.75,0.0};

static tsReal u = 0.f;

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
   glTranslatef(-4.f, 1.5f, 0.0f);

   draw_spline(&spline_orig,yellow,red,green);

   draw_tangent(&spline_orig,&deriv,u,blue);

   glTranslatef(4.f, -1.5f, 0.0f);

   draw_spline(&spline_copy,grey,orange,blue);

   draw_tangent(&spline_orig,&deriv,1.f - u,green);

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
