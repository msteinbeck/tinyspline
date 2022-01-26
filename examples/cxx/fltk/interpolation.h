#include <FL/fl_draw.H>
#include <tinysplinecxx.h>

class Interpolation : public Fl_Widget
{
public:
	Interpolation(int X, int Y, int W, int H, const char *L = NULL)
	: Fl_Widget(X, Y, W, H, L)
	{}

	void draw() {}
};
