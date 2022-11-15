#include "testutils.h"

void
assert_equal_shape_eps(CuTest *tc,
                       tsBSpline *s1,
                       tsBSpline *s2,
                       double eps)
{
	tsDeBoorNet net = ts_deboornet_init();
	tsReal min, max, knot, dist, *s1val = NULL, *s2val = NULL;
	size_t k, dim;
	tsStatus status;

	/* Find minimum dimension. */
	dim = ts_bspline_dimension(s1);
	if (ts_bspline_dimension(s2) < dim)
		dim = ts_bspline_dimension(s2);

	TS_TRY(try, status.code, &status)
		for (k = 0; k < TS_MAX_NUM_KNOTS; k++) {
			/* Eval s1. */
			ts_bspline_domain(s1, &min, &max);
			knot = (tsReal)k / TS_MAX_NUM_KNOTS;
			knot = ( (max - min) * knot ) + min;
			TS_CALL(try, status.code, ts_bspline_eval(
			        s1, knot, &net, &status))
			TS_CALL(try, status.code, ts_deboornet_result(
			        &net, &s1val, &status))
			ts_deboornet_free(&net);

			/* Eval s2. */
			ts_bspline_domain(s2, &min, &max);
			knot = (tsReal)k / TS_MAX_NUM_KNOTS;
			knot = ( (max - min) * knot ) + min;
			TS_CALL(try, status.code, ts_bspline_eval(
			        s2, knot, &net, &status))
			TS_CALL(try, status.code, ts_deboornet_result(
			        &net, &s2val, &status))
			ts_deboornet_free(&net);

			/* Compare results. */
			dist = ts_distance(s1val, s2val, dim);
			CuAssertDblEquals(tc, 0, dist, (tsReal) eps);

			/* Clean up. */
			free(s1val);
			free(s2val);
			s1val = s2val = NULL;
		}
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_deboornet_free(&net);
		free(s1val);
		free(s2val);
	TS_END_TRY
}

void
assert_equal_shape(CuTest *tc,
                   tsBSpline *s1,
                   tsBSpline *s2)
{
	assert_equal_shape_eps(tc, s1, s2, POINT_EPSILON);
}

tsReal
ts_distance_varargs(CuTest *tc,
                    size_t dim,
                    const tsReal *x,
                    double first,
                    ...)
{
	va_list argp;
	tsReal dist, *y = NULL;
	size_t i;

	if (!dim) {
		CuFail(tc, "dim == 0");
		return (tsReal) 0.0;
	}

	y = malloc(dim * sizeof(tsReal));
	if (!y) {
		CuFail(tc, "out of memory");
		return (tsReal) 0.0;
	}

	y[0] = (tsReal) first;
	va_start(argp, first);
	for (i = 1; i < dim; i++)
		y[i] = (tsReal) va_arg(argp, double);
	va_end(argp);

	dist = ts_distance(x, y, dim);
	free(y);
	return dist;
}
