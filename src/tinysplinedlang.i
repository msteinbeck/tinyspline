%module tinysplinedlang

%ignore tinyspline::BSpline::operator=;
%ignore tinyspline::DeBoorNet::operator=;

%include "tinyspline.i"

namespace std {
	%template(Vector) vector<tinyspline::real>;
};
