#include <testutilscxx.h>

void
frames_vectors(CuTest *tc)
{
	// Given
	BSpline spline(7, 2);
	spline.setControlPoints({
			120, 100, // P1
			270, 40,  // P2
			370, 490, // P3
			590, 40,  // P4
			570, 490, // P5
			420, 480, // P6
			220, 500  // P7
		});
	vector<real> knots = spline.uniformKnotSeq(100);

	// When
	FrameSeq frames = spline.computeRMF(knots);

	// Then
	for (size_t i = 0; i < knots.size(); i++) {
		Frame frame = frames.at(i);

		real pos = spline(knots[i]).resultVec3()
		           .distance(frame.position());
		CuAssertDblEquals(tc, 0, pos, POINT_EPSILON);

		real tan = spline.derive()(knots[i]).resultVec3().norm()
		           .distance(frame.tangent());
		CuAssertDblEquals(tc, 0, tan, POINT_EPSILON);

		real norm_tan = frame.normal().angle(frame.tangent());
		CuAssertDblEquals(tc, 90,norm_tan, ANGLE_EPSILON);

		real norm_binorm = frame.normal().angle(frame.binormal());
		CuAssertDblEquals(tc, 90, norm_binorm, ANGLE_EPSILON);

		real binorm_tan = frame.binormal().angle(frame.tangent());
		CuAssertDblEquals(tc, 90, binorm_tan, ANGLE_EPSILON);

		real binorm_norm = frame.binormal().angle(frame.normal());
		CuAssertDblEquals(tc, 90, binorm_norm, ANGLE_EPSILON);
	}
}

CuSuite *
get_frames_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, frames_vectors);
	return suite;
}
