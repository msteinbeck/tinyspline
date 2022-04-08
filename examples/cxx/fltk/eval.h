#include <FL/fl_draw.H>
#include <tinysplinecxx.h>

class Eval : public Fl_Widget
{
public:
	Eval(int X, int Y, int W, int H, const char *L = NULL)
	: Fl_Widget(X, Y, W, H, L), m_degree(3), m_knot(0.1), m_drawNet(true)
	{}

	void draw()
	{
		// Set up the spline to be drawn.
		tinyspline::BSpline spline(7, 2, m_degree);
		std::vector<tinyspline::real> ctrlp = spline.controlPoints();
		ctrlp[0]  = 50;  ctrlp[1]  = 50;  // P1
		ctrlp[2]  = 180; ctrlp[3]  = 30;  // P2
		ctrlp[4]  = 200; ctrlp[5]  = 350; // P3
		ctrlp[6]  = 500; ctrlp[7]  = 30;  // P4
		ctrlp[8]  = 480; ctrlp[9]  = 400; // P5
		ctrlp[10] = 330; ctrlp[11] = 500; // P6
		ctrlp[12] = 50;  ctrlp[13] = 380; // P7
		spline.setControlPoints(ctrlp);

		// Evaluate m_knot.
		tinyspline::DeBoorNet net = spline.eval(m_knot);
		std::vector<tinyspline::real> points = net.points();
		std::vector<tinyspline::real> result = net.result();

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
		if (spline.degree()) {
			// Spline of degree 0 (a sequence of points) is "drawn"
			// solely by its control points (see below).
			fl_color(FL_BLACK);
			fl_begin_line();
			// If spline is a sequence of lines (degree 1), use its
			// control points to draw the lines as in this case
			// sampling is less precise.
			std::vector<tinyspline::real> pts =
				spline.degree() == 1 // sequence of lines
				? spline.controlPoints()
				: spline.sample(100);
			for (size_t i = 0; i < pts.size() / 2; i++)
				fl_vertex(pts[i * 2], pts[i * 2 + 1]);
			fl_end_line();
		}

		// Draw control points.
		double radius = 3;
		for (size_t i = 0; i < ctrlp.size() / 2; i++) {
			tinyspline::real x = ctrlp[i * 2];
			tinyspline::real y = ctrlp[i * 2 + 1];
			fl_rectf(fl_transform_x(x, y) - radius,
			         fl_transform_y(x, y) - radius,
			         radius * 2, radius * 2,
			         FL_RED);
		}

		// Draw De Boor control net.
		if (m_drawNet) {
			size_t numIns = net.numInsertions(), offset = 0;
			for (size_t i = numIns + 1; i > 1; i--) {
				switch (i) {
					case 2: fl_color(FL_MAGENTA);
						break;
					case 3: fl_color(FL_CYAN);
						break;
					case 4: fl_color(FL_GREEN);
						break;
					case 5: fl_color(245, 130, 0);
						break; // orange
					case 6: fl_color(190, 0, 245);
						break; // purple
					case 7: fl_color(FL_LIGHT3);
						break;
					default:
						fl_color(FL_BLACK);
				}
				// Must be set after color.
				fl_line_style(FL_SOLID, 2);
				fl_begin_line();
				for (int j = 0; j < i; j++) {
					fl_vertex(points[offset + j * 2],
					          points[offset + j * 2 + 1]);
				}
				fl_end_line();
				fl_line_style(0);
				offset += i * 2;
			}
		}

		// Draw evaluated point.
		tinyspline::real x = result[0];
		tinyspline::real y = result[1];
		fl_rectf(fl_transform_x(x, y) - radius,
		         fl_transform_y(x, y) - radius,
		         radius * 2, radius * 2,
		         FL_BLUE);

		// Reset translation.
		fl_pop_matrix();
	}

	size_t degree() const
	{
		return m_degree;
	}

	void degree(size_t degree)
	{
		m_degree = degree;
		if (m_degree > 6) m_degree = 6;
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

	bool drawNet() const
	{
		return m_drawNet;
	}

	void drawNet(bool value)
	{
		m_drawNet = value;
	}

private:
	size_t m_degree;
	tinyspline::real m_knot;
	bool m_drawNet;
};
