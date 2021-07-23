#include <cassert>
#include <testutils.h>
#include <tinysplinecxx.h>

using namespace tinyspline;

void run()
{
	BSpline start(7);
	std::vector<real> ctrlp = start.controlPoints();
	//       X                Y
	ctrlp[0]  = 120; ctrlp[1]  = 100;
	ctrlp[2]  = 270; ctrlp[3]  = 40;
	ctrlp[4]  = 370; ctrlp[5]  = 490;
	ctrlp[6]  = 590; ctrlp[7]  = 40;
	ctrlp[8]  = 570; ctrlp[9]  = 490;
	ctrlp[10] = 420; ctrlp[11] = 480;
	ctrlp[12] = 220; ctrlp[13] = 500;
	start.setControlPoints(ctrlp);

	BSpline end(5, 2, 4);
	ctrlp.resize(end.numControlPoints() * end.dimension());
	//       X                Y
	ctrlp[0]  = 60;  ctrlp[1]  = 150;
	ctrlp[2]  = 200; ctrlp[3]  = 300;
	ctrlp[4]  = 370; ctrlp[5]  = 490;
	ctrlp[6]  = 590; ctrlp[7]  = 40;
	ctrlp[8]  = 570; ctrlp[9]  = 490;
	end.setControlPoints(ctrlp);

	Morphism morphism = start.morphTo(end);
	for (size_t i = 0; i <= 10; i++) {
		BSpline morph = morphism((real) i / 10);
		std::string json = morph.toJson();
		morph = morph.tension((tsReal) 0.75).toBeziers().derive();
		assert(morph.domain().min() == start.domain().min());
		assert(morph.domain().max() == start.domain().max());
		assert(morph((tsReal) 0.314).result().size() == 2);
		morph = BSpline(BSpline::parseJson(json));
		assert(morph.sample(100).size() == 200);
	}
}

int main(int argc, char **argv)
{
	run();
	return 0;
}
