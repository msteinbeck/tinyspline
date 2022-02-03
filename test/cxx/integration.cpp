#include <cassert>
#include <cmath>
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

	Vec3 tmp(ctrlp[6], ctrlp[7], (real) 0.0);
	Vec3 vec3 = start.controlPointVec3At(3);
	assert(vec3.distance(tmp) <= POINT_EPSILON);
	vec3.setY(-100);
	start.setControlPointVec3At(3, vec3);
	vec3 = start.controlPointVec3At(3);
	assert(tmp.distance(vec3) > POINT_EPSILON);
	assert(vec3.y() - 100.0 <= POINT_EPSILON);

	std::vector<real> knots = start.knots();
	knots[0] = (tsReal) -1000.0;
	assert(start.knots()[0] <= TS_KNOT_EPSILON);

	knots = start.uniformKnotSeq(20);
	FrameSeq seq = start.computeRMF(knots);
	assert(seq.size() == 20);
	for (size_t i = 0; i < seq.size(); i++) {
		Frame frame = seq.at(i);
		Vec3 pos = start(knots[i]).resultVec3();
		Vec3 tan = start.derive()(knots[i]).resultVec3().norm();
		assert(frame.position().distance(pos) <= POINT_EPSILON);
		assert(frame.tangent().distance(tan) <= POINT_EPSILON);
		real angle_norm_tan = frame.normal().angle(frame.tangent());
		real angle_norm_binorm = frame.normal().angle(frame.binormal());
		real angle_binorm_tan = frame.binormal().angle(frame.tangent());
		real angle_binorm_norm = frame.binormal().angle(frame.normal());
		assert(std::fabs(angle_norm_tan - 90.0) <= ANGLE_EPSILON);
		assert(std::fabs(angle_norm_binorm - 90.0) <= ANGLE_EPSILON);
		assert(std::fabs(angle_binorm_tan - 90.0) <= ANGLE_EPSILON);
		assert(std::fabs(angle_binorm_norm - 90.0) <= ANGLE_EPSILON);
	}

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
