#include <stdlib.h>
#include <tinyspline.h>
#include "CuTest.h"

void to_beziers_issue143(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsBSpline beziers = ts_bspline_init();
	tsStatus status;
	tsReal ctrlp[32] = {
		-8837.0980824304952,
		5410.0643332926420,
		-8837.0980824304952,
		5410.1222121646779,
		-8837.1450025330378,
		5410.1691322672204,
		-8837.2028814355908,
		5410.1691322672204,
		-8837.2607603381439,
		5410.1691322672204,
		-8837.3076804101693,
		5410.1222121646779,
		-8837.3076804101693,
		5410.0643332926420,
		-8837.3076804101693,
		5410.0064544206061,
		-8837.2607603381439,
		5409.9595343485826,
		-8837.2028814355908,
		5409.9595343485826,
		-8837.1634778588214,
		5409.9595343485826,
		-8837.0980824304952,
		5409.9595343485826,
		-8837.0980824304952,
		5409.9595343485826,
		-8837.0980824304952,
		5409.9595343485826,
		-8837.0980824304952,
		5410.0206728110606,
		-8837.0980824304952,
		5410.0643332926420
	};
	tsReal knots[20] = {
		-0.19999998770654204,
		-0.19999998770654204,
		-0.19999998770654204,
		0.00000000000000000,
		0.20000000307336449,
		0.20000000307336449,
		0.20000000307336449,
		0.40000000614672898,
		0.40000000614672898,
		0.40000000614672898,
		0.60000002412125475,
		0.60000002412125475,
		0.60000002412125475,
		0.80000001229345796,
		0.80000001229345796,
		0.80000001229345796,
		1.0000000000000000,
		1.2000000030733644,
		1.2000000030733644,
		1.2000000030733644
	};

	TS_TRY(try, status.code, &status)
		TS_CALL(try, status.code, ts_bspline_new(
			16, 2, 3, TS_CLAMPED, &spline, &status));
		TS_CALL(try, status.code, ts_bspline_set_control_points(
			&spline, ctrlp, &status))
		TS_CALL(try, status.code, ts_bspline_set_knots(
			&spline, knots, &status))
		TS_CALL(try, status.code, ts_bspline_to_beziers(
			&spline, &beziers, &status))
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		ts_bspline_free(&beziers);
	TS_END_TRY

}

CuSuite* get_to_beziers_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, to_beziers_issue143);
	return suite;
}
