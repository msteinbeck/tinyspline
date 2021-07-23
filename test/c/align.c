#include <testutils.h>

void assert_compatible(CuTest *tc, tsBSpline *s1, tsBSpline *s2)
{
	CuAssertIntEquals(tc,
		(int) ts_bspline_degree(s1),
		(int) ts_bspline_degree(s2));
	CuAssertIntEquals(tc,
		(int) ts_bspline_num_control_points(s1),
		(int) ts_bspline_num_control_points(s2));
	CuAssertIntEquals(tc,
		(int) ts_bspline_num_knots(s1),
		(int) ts_bspline_num_knots(s2));
}

void align_point_with_spline(CuTest *tc)
{
	___SETUP___
	tsBSpline point = ts_bspline_init();
	tsBSpline point_aligned = ts_bspline_init();
	tsBSpline spline = ts_bspline_init();

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		1, 4, 0, TS_CLAMPED, &point, &status,
		100.0, 200.0, -100.0, -500.5)) /* P1 */
	C(ts_bspline_new_with_control_points(
		10, 2, 3, TS_CLAMPED, &spline, &status,
		400.0, 600.0,  /* P1 */
		333.0, 450.45, /* P2 */
		120.0, 149.0,  /* P3 */
		300.0, 500.0,  /* P4 */
		-250.0, 499.0, /* P5 */
		-100.0, 466.0, /* P6 */
		0.0, -246.6,   /* P7 */
		0.0, 0.0,      /* P8 */
		0.567, -0.385, /* P9 */
		400.0, 550.0)) /* P10 */

	___WHEN___
	C(ts_bspline_align(&spline, &point, POINT_EPSILON,
		&spline, &point_aligned, &status))

	___THEN___
	CuAssertTrue(tc, point.pImpl != point_aligned.pImpl);
	CuAssertTrue(tc, spline.pImpl != point_aligned.pImpl);
	assert_equal_shape(tc, &point, &point_aligned);
	assert_compatible(tc, &point_aligned, &spline);

	___TEARDOWN___
	ts_bspline_free(&point);
	ts_bspline_free(&spline);
	ts_bspline_free(&point_aligned);
}

void align_line_with_spline(CuTest *tc)
{
	___SETUP___
	tsBSpline line = ts_bspline_init();
	tsBSpline line_aligned = ts_bspline_init();
	tsBSpline spline = ts_bspline_init();

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		2, 3, 1, TS_CLAMPED, &line, &status,
		100.0, 200.0, -10.0,  /* P1 */
		300.0, 400.0, -20.0)) /* P2 */
	C(ts_bspline_new_with_control_points(
		5, 2, 3, TS_CLAMPED, &spline, &status,
		40.0, 10.0,   /* P1 */
		50.0, 150.0,  /* P2 */
		-10, -20,     /* P3 */
		-50, -45,     /* P4 */
		-25.3, -200)) /* P5 */

	___WHEN___
	C(ts_bspline_align(&line, &spline, POINT_EPSILON,
		&line_aligned, &spline, &status))

	___THEN___
	CuAssertTrue(tc, line.pImpl != line_aligned.pImpl);
	CuAssertTrue(tc, spline.pImpl != line_aligned.pImpl);
	assert_equal_shape_eps(tc, &line, &line_aligned, 0.1);
	assert_compatible(tc, &line_aligned, &spline);

	___TEARDOWN___
	ts_bspline_free(&line);
	ts_bspline_free(&spline);
	ts_bspline_free(&line_aligned);
}

void align_spline_to_itself(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	struct tsBSplineImpl *spline_ptr;
	tsBSpline copy = ts_bspline_init();

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		15, 3, 7, TS_CLAMPED, &spline, &status,
		-234.12, 12.4, 12.1,   /* P1 */
		239.53, -423.24, 12.2, /* P2 */
		374.0, 123.3, 12.3,    /* P3 */
		400.0, 600.0, 12.4,    /* P4 */
		333.0, 450.45, 12.5,   /* P5 */
		34.129, 496.31, 12.6,  /* P6 */
		-0.567, -0.385, 12.7,  /* P7 */
		400.0, 550.0, 12.8,    /* P8 */
		-234.129, 496.3, 12.9, /* P9 */
		-234.12, 12.4, 7.1,    /* P10 */
		323.0, 450.45, 7.2,    /* P11 */
		120.0, 149.0, 7.3,     /* P12 */
		120.0, 149.0, 7.4,     /* P13 */
		340.0, 500.0, 7.5,     /* P14 */
		-260.0, 499.0, 7.9))   /* P15 */
	spline_ptr = spline.pImpl;
	C(ts_bspline_copy(&spline, &copy, &status))

	___WHEN___
	C(ts_bspline_align(&spline, &spline, POINT_EPSILON,
		&spline, &spline, &status))

	___THEN___
	CuAssertTrue(tc, spline.pImpl != spline_ptr);
	assert_equal_shape(tc, &spline, &copy);
	assert_compatible(tc, &spline, &copy);


	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_bspline_free(&copy);
}

void align_elevated_with_more_control_points(CuTest *tc)
{
	___SETUP___
	tsBSpline more_control_points = ts_bspline_init();
	struct tsBSplineImpl *more_control_points_ptr;
	tsBSpline more_control_points_copy = ts_bspline_init();

	tsBSpline less_control_points = ts_bspline_init();
	struct tsBSplineImpl *less_control_points_ptr;
	tsBSpline less_control_points_copy = ts_bspline_init();

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		45, 2, 3, TS_CLAMPED, &more_control_points, &status,
		102.0, 596.0,    /* P1 */
		30.0, 14.0,      /* P2 */
		12.0, 69.0,      /* P3 */
		586.0, 35.0,     /* P4 */
		-123.0, -432.4,  /* P5 */
		549.0, -35.34,   /* P6 */
		234.129, 496.31, /* P7 */
		-234.12, 12.4,   /* P8 */
		239.53, -423.24, /* P9 */
		374.0, 123.3,    /* P10 */
		400.0, 600.0,    /* P11 */
		333.0, 450.45,   /* P12 */
		120.0, 149.0,    /* P13 */
		300.0, 500.0,    /* P14 */
		-250.0, 499.0,   /* P15 */
		-100.0, 466.0,   /* P16 */
		0.0, -246.6,     /* P17 */
		0.0, 0.0,        /* P18 */
		0.567, -0.385,   /* P19 */
		400.0, 550.0,    /* P20 */
		234.129, 496.31, /* P21 */
		-234.12, 12.4,   /* P22 */
		239.53, -423.24, /* P23 */
		374.0, 123.3,    /* P24 */
		400.0, 600.0,    /* P25 */
		40.0, 10.0,      /* P26 */
		50.0, 150.0,     /* P27 */
		-10.0, -20.0,    /* P28 */
		-50.0, -45.0,    /* P29 */
		-25.3, -200.0,   /* P30 */
		-586.0, 35.0,    /* P31 */
		-123.0, -432.4,  /* P32 */
		-549.0, -35.34,  /* P33 */
		234.129, 496.31, /* P34 */
		-0.567, -0.385,  /* P35 */
		400.0, 550.0,    /* P36 */
		-234.129, 496.3, /* P37 */
		-234.12, 12.4,   /* P38 */
		323.0, 450.45,   /* P39 */
		120.0, 149.0,    /* P40 */
		340.0, 500.0,    /* P41 */
		-260.0, 499.0,   /* P42 */
		-100.0, 466.0,   /* P43 */
		0.12, -246.6,    /* P44 */
		0.0, -0.1))      /* P45 */
	more_control_points_ptr = more_control_points.pImpl;
	C(ts_bspline_copy(&more_control_points,
		&more_control_points_copy, &status))

	C(ts_bspline_new_with_control_points(
		15, 2, 5, TS_CLAMPED, &less_control_points, &status,
		95.3, -23.24,    /* P1 */
		48.23, 239.93,   /* P2 */
		289.19, 924.4,   /* P3 */
		-24.02, -12.443, /* P4 */
		-49.94, -192.49, /* P5 */
		494.3, 932.24,   /* P6 */
		393.92, 95.24,   /* P7 */
		239.23, -231.23, /* P8 */
		192.123, 32.9,   /* P9 */
		395.94, 294.13,  /* P10 */
		45.23, 895.231,  /* P11 */
		934.921, -482.0, /* P12 */
		94.34, -124.14,  /* P13 */
		485.234, 92.912, /* P14 */
		95.92, -542.84)) /* P15 */
	less_control_points_ptr = less_control_points.pImpl;
	C(ts_bspline_copy(&less_control_points,
		&less_control_points_copy, &status))

	___WHEN___
	C(ts_bspline_align(&more_control_points, &less_control_points,
		POINT_EPSILON, &more_control_points, &less_control_points,
		&status))

	___THEN___
	CuAssertTrue(tc, more_control_points.pImpl != more_control_points_ptr);
	CuAssertTrue(tc, less_control_points.pImpl != less_control_points_ptr);
	assert_equal_shape(tc, &more_control_points,
		&more_control_points_copy);
	assert_equal_shape_eps(tc, &less_control_points,
		&less_control_points_copy, 1.7);
	assert_compatible(tc, &more_control_points,
		&less_control_points);

	___TEARDOWN___
	ts_bspline_free(&more_control_points);
	ts_bspline_free(&more_control_points_copy);
	ts_bspline_free(&less_control_points);
	ts_bspline_free(&less_control_points_copy);
}

CuSuite* get_align_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, align_line_with_spline);
	SUITE_ADD_TEST(suite, align_point_with_spline);
	SUITE_ADD_TEST(suite, align_spline_to_itself);
	SUITE_ADD_TEST(suite, align_elevated_with_more_control_points);
	return suite;
}
