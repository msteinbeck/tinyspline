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
	"capacity|push_back|getitem|setitem|size|reserve|size|getitemcopy") "";

// Create a typemap that generalizes the types float and double to a single type, which is
// accessible with $typemap(cstype, tinyspline::real).
#ifdef TINYSPLINE_FLOAT_PRECISION
	%typemap(cstype) tinyspline::real "float"
#else
	%typemap(cstype) tinyspline::real "double"
#endif

// Change the signature of the C# interface files from std::vector<tinyspline::real> to
// IList<float/double>.
%typemap(cstype) std::vector<tinyspline::real> *
	"System.Collections.Generic.IList<$typemap(cstype, tinyspline::real)>"

// Let the C# interface files redirect the input argument to the converter function.
%typemap(csin) std::vector<tinyspline::real> *
	"RealVector.getCPtr(Utils.ListToVector($csinput))"

// Converts a C# list to a RealVector.
%typemap(cscode) tinyspline::Utils
%{
	internal static RealVector ListToVector(
		System.Collections.Generic.IList<$typemap(cstype, tinyspline::real)> list)
	{
		if (list == null)
			throw new System.ArgumentNullException("List must not be null.");

		RealVector vec = new RealVector(list.Count);
		foreach ($typemap(cstype, tinyspline::real) val in list)
			vec.Add(val);
		return vec;
	}
%}

%include "tinyspline.i"

namespace std {
	%template(RealVector) vector<tinyspline::real>;
};
