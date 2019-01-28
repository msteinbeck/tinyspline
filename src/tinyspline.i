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
%attribute(tinyspline::BSpline,
	size_t,
	degree,
	degree);
%attribute(tinyspline::BSpline,
	size_t,
	order,
	order);
%attribute(tinyspline::BSpline,
	size_t,
	dimension,
	dimension);
%attributeval(tinyspline::BSpline,
	tinyspline::Domain,
	domain,
	domain);
%attributeval(tinyspline::BSpline,
	std::vector<tinyspline::real>,
	controlPoints,
	controlPoints,
	setControlPoints);
%attributeval(tinyspline::BSpline,
	std::vector<tinyspline::real>,
	knots,
	knots,
	setKnots);
%attribute(tinyspline::Domain,
	tinyspline::real,
	min,
	min);
%attribute(tinyspline::Domain,
	tinyspline::real,
	max,
	max);
%attribute(tinyspline::DeBoorNet,
	tinyspline::real,
	knot,
	knot);
%attribute(tinyspline::DeBoorNet,
	size_t,
	index,
	index);
%attribute(tinyspline::DeBoorNet,
	size_t,
	multiplicity,
	multiplicity);
%attribute(tinyspline::DeBoorNet,
	size_t,
	numInsertions,
	numInsertions);
%attribute(tinyspline::DeBoorNet,
	size_t,
	dimension,
	dimension);
%attributeval(tinyspline::DeBoorNet,
	std::vector<tinyspline::real>,
	points,
	points);
%attributeval(tinyspline::DeBoorNet,
	std::vector<tinyspline::real>,
	result,
	result);
#endif

// Make strings directly accessible in target language.
%include "std_string.i"

// Ignore wrapped structs and data fields.
%ignore tsError;
%ignore tsStatus;
%ignore tsDeBoorNet;
%ignore tinyspline::DeBoorNet::data;
%ignore tsBSpline;
%ignore tinyspline::BSpline::data;

// Rename exported enums and enum values.
%rename(BSplineType) tsBSplineType;
%rename(OPENED) TS_OPENED;
%rename(CLAMPED) TS_CLAMPED;
%rename(BEZIERS) TS_BEZIERS;

%{
	#include "tinyspline.h"
	#include "tinysplinecpp.h"
%}

%include "tinyspline.h"
%include "tinysplinecpp.h"

// Make std::vector<tinyspline::real> available in target language. Is also required by
// '%attributeval' (see above).
%include "std_vector.i"
