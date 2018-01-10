%module tinysplinecsharp

// Create a typemap that generalizes the types float and double to a single type accessible with
// $typemap(cstype, tinyspline::real).
#ifdef TINYSPLINE_FLOAT_PRECISION
	%typemap(cstype) tinyspline::real "float"
#else
	%typemap(cstype) tinyspline::real "double"
#endif

// Change the signature of the C# interface files from std::vector<tinyspline::real> to
// IList<float/double>.
%typemap(cstype) std::vector<tinyspline::real> *
	"System.Collections.Generic.IList<$typemap(cstype, tinyspline::real)>"

// Let C# interface files redirect the input argument to the converter function.
%typemap(csin) std::vector<tinyspline::real> *
	"RationalVector.getCPtr(Utils.ListToVector($csinput))"

//********************************************************
//*                                                      *
//* Utils (C#)                                           *
//*                                                      *
//********************************************************
%typemap(cscode) tinyspline::Utils
%{
	internal static RationalVector ListToVector(
		System.Collections.Generic.IList<$typemap(cstype, tinyspline::real)> list)
	{
		if (list == null)
			throw new System.ArgumentNullException("List must not be null.");

		RationalVector vec = new RationalVector(list.Count);
		foreach ($typemap(cstype, tinyspline::real) val in list)
			vec.Add(val);
		return vec;
	}
%}

//********************************************************
//*                                                      *
//* BSpline (C#)                                         *
//*                                                      *
//********************************************************
%ignore tinyspline::BSpline::operator();
%ignore tinyspline::BSpline::operator=;

//********************************************************
//*                                                      *
//* DeBoorNet (C#)                                       *
//*                                                      *
//********************************************************
%ignore tinyspline::DeBoorNet::operator=;

//********************************************************
//*                                                      *
//* SWIG base file                                       *
//*                                                      *
//********************************************************
%include "tinyspline.i"

namespace std {
	%template(RationalVector) vector<tinyspline::real>;
};
