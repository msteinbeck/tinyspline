#include <FL/fl_draw.H>
#include "tinysplinecxx.h"

class Sample : public Fl_Widget
{
public:
	Sample(int X, int Y, int W, int H, const char *L = NULL)
	: Fl_Widget(X, Y, W, H, L), m_num(50), m_type(TS_CLAMPED),
	m_drawPoints(false)
	{}

	void draw()
	{
		// Set up spline.
		tinyspline::BSpline spline(7, 2, 3, m_type);
		std::vector<tinyspline::real> ctrlp = spline.controlPoints();
		ctrlp[0]  = 50;  ctrlp[1]  = 100; // P1
		ctrlp[2]  = 200; ctrlp[3]  = 80;  // P2
		ctrlp[4]  = 300; ctrlp[5]  = 450; // P3
		ctrlp[6]  = 520; ctrlp[7]  = 80;  // P4
		ctrlp[8]  = 500; ctrlp[9]  = 450; // P5
		ctrlp[10] = 350; ctrlp[11] = 550; // P6
		ctrlp[12] = 50;  ctrlp[13] = 500; // P7
		spline.setControlPoints(ctrlp);

		// Clear background.
		fl_color(FL_WHITE);
		fl_rectf(x(), y(), w(), h());

		// Draw spline.
		fl_color(FL_BLACK);
		fl_begin_line();
		std::vector<tinyspline::real> pts = spline.sample(m_num);
		for (size_t i = 0; i < pts.size() / 2; i++)
			fl_vertex(pts[i * 2], pts[i * 2 + 1]);
		fl_end_line();

		// Draw sampled points.
		if (m_drawPoints) {
			fl_color(FL_BLUE);
			for (size_t i = 0; i < pts.size() / 2; i++) {
				fl_rectf(
					(int) pts[i * 2] - 2,
					(int) pts[i * 2 + 1] - 2,
					4, 4
					);
			}
		}
	}

	size_t num() const
	{
		return m_num;
	}

	void num(size_t num)
	{
		m_num = num;
	}

	tinyspline::BSpline::type knotVector() const
	{
		return m_type;
	}

	void knotVector(tinyspline::BSpline::type type)
	{
		m_type = type;
		if (m_type != TS_CLAMPED && m_type != TS_OPENED)
			m_type = TS_CLAMPED;
	}

	bool drawPoints() const
	{
		return m_drawPoints;
	}

	void drawPoints(bool drawPoints) {
		m_drawPoints = drawPoints;
	}

private:
	size_t m_num;
	tinyspline::BSpline::type m_type;
	bool m_drawPoints;
};