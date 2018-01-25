#ifndef TINYSPLINE_UTILS_H
#define TINYSPLINE_UTILS_H

struct tsBspline;
struct CuTest;

void ctests_init_non_default_bspline(tsBSpline *bspline);

void ctests_assert_default_bspline(CuTest *tc, tsBSpline *bspline);

#endif
