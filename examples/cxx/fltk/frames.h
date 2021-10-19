#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include "tinysplinecxx.h"

class Frames : public Fl_Gl_Window
{
public:
	Frames(int X, int Y, int W, int H, const char *L = NULL)
	: Fl_Gl_Window(X, Y, W, H, L), m_knot(0.0)
	{
		mode(FL_RGB | FL_ALPHA | FL_DEPTH | FL_DOUBLE);
	}

	void draw() {
		if (!valid()) {
			glLoadIdentity();
			glViewport(0, 0, w(), h());
			glOrtho(-w(), w(), -h(), h(), -200, 200);
		}

		tinyspline::BSpline spline(11, 3, 3);
		std::vector<tinyspline::real> ctrlp = spline.controlPoints();
		ctrlp[0]  = -300; ctrlp[1]  = 0; ctrlp[2]    = 0;    // P1
		ctrlp[3]  = -100; ctrlp[4]  = -80; ctrlp[5]  = -100; // P2
		ctrlp[6]  = 0;    ctrlp[7]  = 350; ctrlp[8]  = -80;  // P3
		ctrlp[9]  = 420;  ctrlp[10] = -80; ctrlp[11] = -70;  // P4
		ctrlp[12] = 400;  ctrlp[13] = 350; ctrlp[14] = -20;  // P5
		ctrlp[15] = 250;  ctrlp[16] = 450; ctrlp[17] = -10;  // P6
		ctrlp[18] = -50;  ctrlp[19] = 400; ctrlp[20] = 50;   // P7
		ctrlp[21] = -30;  ctrlp[22] = 300; ctrlp[23] = 80;   // P8
		ctrlp[24] = 0;    ctrlp[25] = 200; ctrlp[26] = 90;   // P9
		ctrlp[27] = 30;   ctrlp[28] = 230; ctrlp[29] = 10;   // P10
		ctrlp[30] = 80;   ctrlp[31] = 300; ctrlp[32] = 0;    // P11
		spline.setControlPoints(ctrlp);

		// Clear screen.
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // white
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw spline.
		glLineWidth(1);
		glColor3f(0.f, 0.0f, 0.0f); // black
		glBegin(GL_LINE_STRIP);
		std::vector<tinyspline::real> pts = spline.sample(100);
		for (size_t i = 1; i < pts.size() / 3; i++) {
				glVertex3d(pts[(i-1) * 3],
					   pts[(i-1) * 3 + 1],
					   pts[(i-1) * 3 + 2]);
				glVertex3d(pts[i * 3],
					   pts[i * 3 + 1],
					   pts[i * 3 + 2]);
		}
		glEnd();

		// Draw frames.
		glLineWidth(2);
		std::vector<tinyspline::real> knots =
			spline.uniformKnotSeq(1001);
		tinyspline::FrameSeq seq = spline.computeRMF(knots);
		size_t t = m_knot * 1000;
		tinyspline::Vec3 pos = seq.at(t).position();

		glColor3f(1.f, 0.0f, 0.0f); // red
		glBegin(GL_LINE_STRIP); // tangent
			glVertex3f(pos.x(), pos.y(), pos.z());
			tinyspline::Vec3 tan = seq.at(t).tangent();
			tan = pos + tan * 100;
			glVertex3f(tan.x(), tan.y(), tan.z());
		glEnd();

		glColor3f(0.f, 1.0f, 0.0f); // green
		glBegin(GL_LINE_STRIP); // binormal
			glVertex3f(pos.x(), pos.y(), pos.z());
			tinyspline::Vec3 bin = seq.at(t).binormal();
			bin = pos + bin * 100;
			glVertex3f(bin.x(), bin.y(), bin.z());
		glEnd();

		glColor3f(0.f, 0.0f, 1.0f); // blue
		glBegin(GL_LINE_STRIP); // normal
			glVertex3f(pos.x(), pos.y(), pos.z());
			tinyspline::Vec3 nor = seq.at(t).normal();
			nor = pos + nor * 100;
			glVertex3f(nor.x(), nor.y(), nor.z());
		glEnd();
	}

	tinyspline::real knot() const
	{
		return m_knot;
	}

	void knot(double knot)
	{
		m_knot = knot;
		if (m_knot < 0.0) m_knot = 0.0;
		if (m_knot > 1.0) m_knot = 1.0;
	}

private:
	tinyspline::real m_knot;
};
