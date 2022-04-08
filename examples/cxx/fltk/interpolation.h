#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <tinysplinecxx.h>

class Interpolation : public Fl_Widget
{
public:
	enum Type { CUBIC_NATURAL, CATMULL_ROM };

	void reset()
	{
		m_points.clear();
		m_points.push_back(tinyspline::Vec2( 50, 200));
		m_points.push_back(tinyspline::Vec2(200, 100));
		m_points.push_back(tinyspline::Vec2(400, 120));
		m_points.push_back(tinyspline::Vec2(450, 300));
		m_points.push_back(tinyspline::Vec2(210, 250));
		m_points.push_back(tinyspline::Vec2(300, 400));
		m_points.push_back(tinyspline::Vec2(170, 430));
		m_alpha = 0.5;
	}

	Interpolation(int X, int Y, int W, int H, const char *L = NULL)
	: Fl_Widget(X, Y, W, H, L), m_points(), m_selected(-1),
	  m_drawCubicNatural(true), m_drawCatmullRom(true), m_alpha()
	{
		reset();
	}

	void drawInterpolatedSpline(Type type)
	{
		std::vector<tinyspline::real> points;
		for (const auto& v : m_points) {
			points.push_back(v.x());
			points.push_back(v.y());
		}
		tinyspline::BSpline spline;
		Fl_Color color;
		int style;
		if (type == CUBIC_NATURAL) {
			spline = tinyspline::BSpline::
			         interpolateCubicNatural(points, 2);
			color = FL_BLACK;
			style = FL_SOLID;
		}
		else if (type == CATMULL_ROM) {
			spline = tinyspline::BSpline::
			         interpolateCatmullRom(points, 2, m_alpha);
			color = FL_DARK_BLUE;
			style = FL_DASH;
		}
		else return;

		fl_color(color);
		fl_line_style(style);
		fl_begin_line();
		std::vector<tinyspline::real> pts = spline.sample(100);
		for (size_t i = 0; i < pts.size() / 2; i++) {
			tinyspline::real x = this->x() + pts[i * 2];
			tinyspline::real y = this->y() + pts[i * 2 + 1];
			fl_vertex(x, y);
		}
		fl_end_line();
		fl_line_style(0);
	}

	void draw()
	{
		// Clear background.
		fl_color(FL_WHITE);
		fl_rectf(x(), y(), w(), h());

		// Draw Points.
		double radius = 3;
		for (const auto& v : m_points) {
			tinyspline::real x = this->x() + v.x();
			tinyspline::real y = this->y() + v.y();
			fl_rectf(x - radius,
			         y - radius,
			         radius * 2, radius * 2,
			         FL_RED);
		}

		// Draw spline.
		if (m_drawCubicNatural)
			drawInterpolatedSpline(CUBIC_NATURAL);
		if (m_drawCatmullRom)
			drawInterpolatedSpline(CATMULL_ROM);
	}

	int handle(int event)
	{
		tinyspline::Vec2 pt(Fl::event_x() - x(),
		                    Fl::event_y() - y());
		tinyspline::real min = 7, dist;
		switch(event) {
		case FL_ENTER:
			return 1;
		case FL_MOVE:
			m_selected = -1;
			for (int i = 0; i < m_points.size(); i++) {
				dist = m_points[i].distance(pt);
				if (dist < min) {
					m_selected = i;
					min = dist;
				}
			}
			if (m_selected >= 0)
				window()->cursor(FL_CURSOR_HAND);
			else
				window()->cursor(FL_CURSOR_DEFAULT);
			return 1;
		case FL_PUSH:
			return m_selected >= 0 ? 1 : 0;
		case FL_DRAG:
			m_points[m_selected] = pt;
			redraw();
			return 1;
		default:
			return Fl_Widget::handle(event);
		}
	}

	bool drawCubicNatural() const
	{
		return m_drawCubicNatural;
	}

	void drawCubicNatural(bool value)
	{
		m_drawCubicNatural = value;
	}

	bool drawCatmullRom() const
	{
		return m_drawCatmullRom;
	}

	void drawCatmullRom(bool value)
	{
		m_drawCatmullRom = value;
	}

	tinyspline::real alpha() const
	{
		return m_alpha;
	}

	void alpha(double alpha)
	{
		m_alpha = alpha;
	}

private:
	std::vector<tinyspline::Vec2> m_points;
	int m_selected;
	bool m_drawCubicNatural, m_drawCatmullRom;
	tinyspline::real m_alpha;
};
