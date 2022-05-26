#include <testutilscxx.h>
#include <stdexcept>

void
assert_equals(CuTest *tc,
              const Frame &f1,
              const Frame &f2)
{
	real pos = f1.position().distance(f2.position());
	CuAssertDblEquals(tc, 0, pos, POINT_EPSILON);
	real tan = f1.tangent().distance(f2.tangent());
	CuAssertDblEquals(tc, 0, tan, POINT_EPSILON);
	real norm = f1.normal().distance(f2.normal());
	CuAssertDblEquals(tc, 0, norm, POINT_EPSILON);
	real binorm = f1.binormal().distance(f2.binormal());
	CuAssertDblEquals(tc, 0, binorm, POINT_EPSILON);
}

void
assert_equals(CuTest *tc,
              const FrameSeq &fs1,
              const FrameSeq &fs2)
{
	CuAssertIntEquals(tc, (int) fs1.size(), (int) fs2.size());
	for (size_t i = 0; i < fs1.size(); i++)
		assert_equals(tc, fs1.at(i), fs2.at(i));
}

void
assert_orthogonal(CuTest *tc,
                  const Vec3 &v1,
                  const Vec3 &v2)
{
	real angle = v1.angle(v2);
	CuAssertDblEquals(tc, 90, angle, ANGLE_EPSILON);
}

void
frames_frameseq_copy_ctor(CuTest *tc)
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
	FrameSeq frames = spline.computeRMF(
		spline.uniformKnotSeq(10));

	// When
	FrameSeq copy(frames);

	// Then
	assert_equals(tc, frames, copy);
}

void
frames_frameseq_move_ctor(CuTest *tc)
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
	FrameSeq frames = spline.computeRMF(
		spline.uniformKnotSeq(10));
	FrameSeq toBeMoved(frames);

	// When
	FrameSeq move(std::move(toBeMoved));

	// Then
	assert_equals(tc, frames, move);
	CuAssertIntEquals(tc, 0, (int) toBeMoved.size());
}

void
frames_frameseq_copy_assign(CuTest *tc)
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
	FrameSeq frames = spline.computeRMF(
		spline.uniformKnotSeq(10));
	FrameSeq copy;

	// When
	copy = frames;

	// Then
	assert_equals(tc, frames, copy);
}

void
frames_frameseq_move_assign(CuTest *tc)
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
	FrameSeq frames = spline.computeRMF(
		spline.uniformKnotSeq(10));
	FrameSeq toBeMoved(frames);
	FrameSeq move;

	// When
	move = std::move(toBeMoved);

	// Then
	assert_equals(tc, frames, move);
	CuAssertIntEquals(tc, 0, (int) toBeMoved.size());
}

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

		real pos = spline(knots[i])
		           .resultVec3()
		           .distance(frame.position());
		CuAssertDblEquals(tc, 0, pos, POINT_EPSILON);

		real tan = spline.derive()(knots[i])
		           .resultVec3()
		           .normalize()
		           .distance(frame.tangent());
		CuAssertDblEquals(tc, 0, tan, POINT_EPSILON);

		assert_orthogonal(tc, frame.normal(), frame.tangent());
		assert_orthogonal(tc, frame.normal(), frame.binormal());
		assert_orthogonal(tc, frame.binormal(), frame.tangent());
	}
}

void
frames_frameseq_invalid_idx(CuTest *tc)
{
	FrameSeq frames;

	try {
		frames.at(1);
	} catch (out_of_range &ex) {
		string what(ex.what());
		CuAssertTrue(tc, what.length() > 0);
		return;
	}
	CuFail(tc, "Expected exception");
}

void
frames_frameseq_to_string(CuTest *tc)
{
	// When
	FrameSeq frames;

	// When
	string s = frames.toString();

	// Then
	CuAssertTrue(tc, s.length() > 0);
}

CuSuite *
get_frames_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, frames_vectors);
	SUITE_ADD_TEST(suite, frames_frameseq_copy_ctor);
	SUITE_ADD_TEST(suite, frames_frameseq_move_ctor);
	SUITE_ADD_TEST(suite, frames_frameseq_copy_assign);
	SUITE_ADD_TEST(suite, frames_frameseq_move_assign);
	SUITE_ADD_TEST(suite, frames_frameseq_invalid_idx);
	SUITE_ADD_TEST(suite, frames_frameseq_to_string);
	return suite;
}
