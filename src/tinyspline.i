// NOTE:
//
// This file configures all language independent settings. It gets included in each concrete
// module with:
//
// 	%include "tinyspline.i"
//
// For some reason you MUST include this file AFTER custom typemaps.

// Add exception handling.
%include "exception.i"
%exception {
	try {
		$action
	} catch (const std::out_of_range& e) {
		SWIG_exception(SWIG_IndexError, e.what());
	} catch (const std::runtime_error& e) {
		SWIG_exception(SWIG_RuntimeError, e.what());
	}
}

// Generate getter and setter.
#ifndef SWIGD
%include <attribute.i>
%attribute(tinyspline::BSpline, size_t, deg, deg);
%attribute(tinyspline::BSpline, size_t, order, order);
%attribute(tinyspline::BSpline, size_t, dim, dim);
%attribute(tinyspline::BSpline, size_t, nCtrlp, nCtrlp);
%attribute(tinyspline::BSpline, size_t, nKnots, nKnots);
%attributeval(tinyspline::BSpline, std::vector<tinyspline::real>, ctrlp, ctrlp, setCtrlp);
%attributeval(tinyspline::BSpline, std::vector<tinyspline::real>, knots, knots, setKnots);
%attribute(tinyspline::DeBoorNet, tinyspline::real, u, u);
%attribute(tinyspline::DeBoorNet, size_t, k, k);
%attribute(tinyspline::DeBoorNet, size_t, s, s);
%attribute(tinyspline::DeBoorNet, size_t, h, h);
%attribute(tinyspline::DeBoorNet, size_t, dim, dim);
%attribute(tinyspline::DeBoorNet, size_t, nPoints, nPoints);
%attributeval(tinyspline::DeBoorNet, std::vector<tinyspline::real>, points, points);
%attributeval(tinyspline::DeBoorNet, std::vector<tinyspline::real>, result, result);
#endif

// Make strings directly accessible in target language.
%include "std_string.i"

// Ignore wrapped structs and data fields.
%ignore tsError;
%ignore tsDeBoorNet;
%ignore tinyspline::DeBoorNet::data;
%ignore tsBSpline;
%ignore tinyspline::BSpline::data;
// Ignore move semantics.
%ignore tinyspline::DeBoorNet::DeBoorNet(DeBoorNet &&);
%ignore tinyspline::swap(DeBoorNet &, DeBoorNet &);
%ignore tinyspline::BSpline::BSpline(BSpline &&);
%ignore tinyspline::swap(BSpline &, BSpline &);

// Rename exported enums and enum values.
%rename(BSplineType) tsBSplineType;
%rename(OPENED) TS_OPENED;
%rename(CLAMPED) TS_CLAMPED;
%rename(BEZIERS) TS_BEZIERS;
%rename(NONE) TS_NONE;

%{
	#include "tinyspline.h"
	#include "tinysplinecpp.h"
%}

%include "tinyspline.h"
%include "tinysplinecpp.h"

// Make std::vector<tinyspline::real> available in target language. Is also required by
// '%attributeval' (see above).
%include "std_vector.i"
