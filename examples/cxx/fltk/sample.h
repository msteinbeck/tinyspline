#include <FL/fl_draw.H>
#include <tinysplinecxx.h>

class Sample : public Fl_Widget
{
public:
	Sample(int X, int Y, int W, int H, const char *L = NULL)
	: Fl_Widget(X, Y, W, H, L), m_num(50),
	m_type(tinyspline::BSpline::Type::Clamped),
	m_drawPoints(false), m_drawControlPoints(true)
	{}

	void draw()
	{
		// Set up the spline to be drawn.
		tinyspline::BSpline spline(7, 2, 3, m_type);
		std::vector<tinyspline::real> ctrlp = spline.controlPoints();
		ctrlp[0]  = 50;  ctrlp[1]  = 50;  // P1
		ctrlp[2]  = 180; ctrlp[3]  = 30;  // P2
		ctrlp[4]  = 200; ctrlp[5]  = 350; // P3
		ctrlp[6]  = 500; ctrlp[7]  = 30;  // P4
		ctrlp[8]  = 480; ctrlp[9]  = 400; // P5
		ctrlp[10] = 330; ctrlp[11] = 500; // P6
		ctrlp[12] = 50;  ctrlp[13] = 380; // P7
		spline.setControlPoints(ctrlp);

		// Make drawing functions relative to widget. Points (x, y
		// coordinates) must be transformed using `fl_transform_x' and
		// `fl_transform_y'.
		fl_push_matrix();
		fl_translate(x(), y());

		// Clear background.
		fl_color(FL_WHITE);
		fl_rectf(fl_transform_x(0, 0),
		         fl_transform_y(0, 0),
		         w(), h());

		// Draw spline.
		fl_color(FL_BLACK);
		fl_begin_line();
		std::vector<tinyspline::real> pts = spline.sample(m_num);
		for (size_t i = 0; i < pts.size() / 2; i++)
			fl_vertex(pts[i * 2], pts[i * 2 + 1]);
		fl_end_line();

		// Draw control points.
		if (m_drawControlPoints) {
			double radius = 3;
			for (size_t i = 0; i < ctrlp.size() / 2; i++) {
				tinyspline::real x = ctrlp[i * 2];
				tinyspline::real y = ctrlp[i * 2 + 1];
				fl_rectf(fl_transform_x(x, y) - radius,
				         fl_transform_y(x, y) - radius,
				         radius * 2, radius * 2,
				         FL_RED);
			}
		}

		// Draw sampled points.
		if (m_drawPoints) {
			double radius = 2.0;
			for (size_t i = 0; i < pts.size() / 2; i++) {
				tinyspline::real x = (int) pts[i * 2];
				tinyspline::real y = (int) pts[i * 2 + 1];
				fl_rectf(fl_transform_x(x, y) - radius,
				         fl_transform_y(x, y) - radius,
				         radius * 2, radius * 2,
				         FL_BLUE);
			}
		}

		// Reset translation.
		fl_pop_matrix();
	}

	size_t num() const
	{
		return m_num;
	}

	void num(size_t num)
	{
		m_num = num;
	}

	tinyspline::BSpline::Type knotVector() const
	{
		return m_type;
	}

	void knotVector(tinyspline::BSpline::Type type)
	{
		if (m_type == tinyspline::BSpline::Type::Clamped ||
		    m_type == tinyspline::BSpline::Type::Opened)
			m_type = type;
	}

	bool drawPoints() const
	{
		return m_drawPoints;
	}

	void drawPoints(bool value) {
		m_drawPoints = value;
	}

	bool drawControlPoints() const
	{
		return m_drawControlPoints;
	}

	void drawControlPoints(bool value)
	{
		m_drawControlPoints = value;
	}

private:
	size_t m_num;
	tinyspline::BSpline::Type m_type;
	bool m_drawPoints;
	bool m_drawControlPoints;
};
