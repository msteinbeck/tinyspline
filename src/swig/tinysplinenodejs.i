%module tinysplinenodejs

%ignore tinyspline::BSpline::operator();
%ignore tinyspline::BSpline::operator=;
%ignore tinyspline::DeBoorNet::operator=;
%ignore tinyspline::Domain::operator=;
%ignore tinyspline::Morphism::operator();

%include "tinyspline.i"

namespace std {
	%template(Vector) vector<tinyspline::real>;
};
