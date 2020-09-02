%module tinysplinecsharp

%ignore tinyspline::BSpline::operator();
%ignore tinyspline::BSpline::operator=;
%ignore tinyspline::DeBoorNet::operator=;
%ignore tinyspline::Domain::operator=;

%csmethodmodifiers tinyspline::BSpline::toString "public override";
%csmethodmodifiers tinyspline::DeBoorNet::toString "public override";
%csmethodmodifiers tinyspline::Domain::toString "public override";

// Capitalize first character of methods and properties.
%rename("%(camelcase)s", %$ismember, notregexmatch$name=
	// Ignore some methods of std::vector.
	"capacity|push_back|getitem|setitem|size|reserve|getitemcopy") "";

// Create a typemap that generalizes the types float and double to a single
// representation which is accessible with $typemap(cstype, tinyspline::real).
#ifdef TINYSPLINE_FLOAT_PRECISION
	%typemap(cstype) tinyspline::real "float"
#else
	%typemap(cstype) tinyspline::real "double"
#endif

// Change the signature of the C# interface files from
// std::vector<tinyspline::real> to IList<float/double>.
%typemap(cstype) std::vector<tinyspline::real> *
	"System.Collections.Generic.IList<$typemap(cstype, tinyspline::real)>"

// Create RealVector from IList so that regular C# lists can be passed as
// argument.
%typemap(csin) std::vector<tinyspline::real> *
	"RealVector.getCPtr(new RealVector($csinput))"

// Fix memory ownership of non-property methods returning
// std::vector<tinyspline::real> as pointer.
%newobject tinyspline::BSpline::controlPointAt;
%newobject tinyspline::BSpline::evalAll;
%newobject tinyspline::BSpline::sample;

%include "tinyspline.i"

namespace std {
	%template(RealVector) vector<tinyspline::real>;
};
