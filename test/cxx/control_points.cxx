#include <testutilscxx.h>

void
control_points_set_vec4_at_dim5(CuTest* tc)
{
	// Given
	BSpline spline(1, 5, 0);
	vector<real> ctrlp = spline.controlPoints();
	spline.setControlPoints({ 0.0, 0.0, 0.0, 0.0, 0.0 });
	ctrlp = spline.controlPoints();
	CuAssertDblEquals(tc, 0.0,  ctrlp[0],  POINT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  ctrlp[1],  POINT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  ctrlp[2],  POINT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  ctrlp[3],  POINT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  ctrlp[4],  POINT_EPSILON);

	// When
	Vec4 vec(1.0, 2.0, 3.0, 4.0);
	spline.setControlPointVec4At(0, vec);

	// Then
	ctrlp = spline.controlPoints();
	CuAssertDblEquals(tc, 1.0,  ctrlp[0],  POINT_EPSILON);
	CuAssertDblEquals(tc, 2.0,  ctrlp[1],  POINT_EPSILON);
	CuAssertDblEquals(tc, 3.0,  ctrlp[2],  POINT_EPSILON);
	CuAssertDblEquals(tc, 4.0,  ctrlp[3],  POINT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  ctrlp[4],  POINT_EPSILON);
}

void
control_points_set_vec2_at_dim5(CuTest* tc)
{
	// Given
	BSpline spline(1, 5, 0);
	vector<real> ctrlp = spline.controlPoints();
	spline.setControlPoints({ 0.0, 0.0, 0.0, 0.0, 0.0 });
	ctrlp = spline.controlPoints();
	CuAssertDblEquals(tc, 0.0,  ctrlp[0],  POINT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  ctrlp[1],  POINT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  ctrlp[2],  POINT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  ctrlp[3],  POINT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  ctrlp[4],  POINT_EPSILON);

	// When
	Vec2 vec(1.0, 2.0);
	spline.setControlPointVec2At(0, vec);

	// Then
	ctrlp = spline.controlPoints();
	CuAssertDblEquals(tc, 1.0,  ctrlp[0],  POINT_EPSILON);
	CuAssertDblEquals(tc, 2.0,  ctrlp[1],  POINT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  ctrlp[2],  POINT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  ctrlp[3],  POINT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  ctrlp[4],  POINT_EPSILON);
}

CuSuite *
get_control_points_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, control_points_set_vec4_at_dim5);
	SUITE_ADD_TEST(suite, control_points_set_vec2_at_dim5);
	return suite;
}
