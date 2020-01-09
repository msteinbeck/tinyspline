%module tinysplinedlang

%ignore tinyspline::BSpline::operator=;
%ignore tinyspline::DeBoorNet::operator=;
%ignore tinyspline::Domain::operator=;

%dmethodmodifiers tinyspline::BSpline::toString "override public";
%dmethodmodifiers tinyspline::DeBoorNet::toString "override public";
%dmethodmodifiers tinyspline::Domain::toString "override public";

%include "tinyspline.i"

namespace std {
	%template(Vector) vector<tinyspline::real>;
};
