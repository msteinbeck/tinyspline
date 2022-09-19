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
%attribute(tinyspline::BSpline,
	size_t,
	numControlPoints,
	numControlPoints);
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
%attributeval(tinyspline::ChordLengths,
	tinyspline::BSpline,
	spline,
	spline);
%attributeval(tinyspline::ChordLengths,
	std::vector<tinyspline::real>,
	knots,
	knots);
%attributeval(tinyspline::ChordLengths,
	std::vector<tinyspline::real>,
	values,
	values);
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
%attribute(tinyspline::Domain,
	tinyspline::real,
	min,
	min);
%attribute(tinyspline::Domain,
	tinyspline::real,
	max,
	max);
%attributeval(tinyspline::Frame,
	tinyspline::Vec3,
	position,
	position);
%attributeval(tinyspline::Frame,
	tinyspline::Vec3,
	tangent,
	tangent);
%attributeval(tinyspline::Frame,
	tinyspline::Vec3,
	normal,
	normal);
%attributeval(tinyspline::Frame,
	tinyspline::Vec3,
	binormal,
	binormal);
%attributeval(tinyspline::Morphism,
	tinyspline::BSpline,
	origin,
	origin);
%attributeval(tinyspline::Morphism,
	tinyspline::BSpline,
	target,
	target);
%attribute(tinyspline::Morphism,
	tinyspline::real,
	epsilon,
	epsilon);
%attribute(tinyspline::Vec2,
	tinyspline::real,
	x,
	x,
	setX);
%attribute(tinyspline::Vec2,
	tinyspline::real,
	y,
	y,
	setY);
%attributeval(tinyspline::Vec2,
	std::vector<tinyspline::real>,
	values,
	values);
%attribute(tinyspline::Vec3,
	tinyspline::real,
	x,
	x,
	setX);
%attribute(tinyspline::Vec3,
	tinyspline::real,
	y,
	y,
	setY);
%attribute(tinyspline::Vec3,
	tinyspline::real,
	z,
	z,
	setZ);
%attributeval(tinyspline::Vec3,
	std::vector<tinyspline::real>,
	values,
	values);
%attribute(tinyspline::Vec4,
	tinyspline::real,
	x,
	x,
	setX);
%attribute(tinyspline::Vec4,
	tinyspline::real,
	y,
	y,
	setY);
%attribute(tinyspline::Vec4,
	tinyspline::real,
	z,
	z,
	setZ);
%attribute(tinyspline::Vec4,
	tinyspline::real,
	w,
	w,
	setW);
%attributeval(tinyspline::Vec4,
	std::vector<tinyspline::real>,
	values,
	values);
#endif

// Make strings directly accessible in target language.
%include "std_string.i"

// Ignore C elements and unsupported C++ features.
%rename("$ignore", regexmatch$name="ts_") "";
%ignore tsBSpline;
%ignore tsBSplineType;
%ignore tsDeBoorNet;
%ignore tsError;
%ignore tsFrame;
%ignore tsStatus;
%ignore tinyspline::BSpline::BSpline(BSpline &&);
%ignore tinyspline::BSpline::operator=;
%ignore tinyspline::ChordLengths::ChordLengths(ChordLengths &&);
%ignore tinyspline::ChordLengths::operator=;
%ignore tinyspline::DeBoorNet::DeBoorNet(DeBoorNet &&);
%ignore tinyspline::DeBoorNet::operator=;
%ignore tinyspline::Domain::operator=;
%ignore tinyspline::Frame::operator=;
%ignore tinyspline::FrameSeq::FrameSeq(FrameSeq &&);
%ignore tinyspline::FrameSeq::operator=;
%ignore tinyspline::Vec2::operator=;
%ignore tinyspline::Vec3::operator=;
%ignore tinyspline::Vec4::operator=;


%{
	#include "tinyspline.h"
	#include "tinysplinecxx.h"
%}

%include "tinyspline.h"
%include "tinysplinecxx.h"

// Make std::vector<tinyspline::real> available in target language. Is also required by
// '%attributeval' (see above).
%include "std_vector.i"
