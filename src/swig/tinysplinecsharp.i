%module tinysplinecsharp

// Proper enums.
%include "enums.swg"

%ignore tinyspline::BSpline::operator();
%ignore tinyspline::Morphism::operator();

%ignore tinyspline::Vec2::operator+;
%ignore tinyspline::Vec2::operator-;
%ignore tinyspline::Vec2::operator*;

%typemap(cscode,noblock=1) tinyspline::Vec2 {
	public static Vec2 operator+(Vec2 a, Vec2 b)
	{ return a.Add(b); }

	public static Vec2 operator-(Vec2 a, Vec2 b)
	{ return a.Subtract(b); }

#ifndef TINYSPLINE_FLOAT_PRECISION
	public static Vec2 operator*(Vec2 vec, double val)
	{ return vec.Multiply(val); }
#endif

	public static Vec2 operator*(Vec2 vec, float val)
	{ return vec.Multiply(val); }
}

%ignore tinyspline::Vec3::operator+;
%ignore tinyspline::Vec3::operator-;
%ignore tinyspline::Vec3::operator*;

%typemap(cscode,noblock=1) tinyspline::Vec3 {
	public static Vec3 operator+(Vec3 a, Vec3 b)
	{ return a.Add(b); }

	public static Vec3 operator-(Vec3 a, Vec3 b)
	{ return a.Subtract(b); }

#ifndef TINYSPLINE_FLOAT_PRECISION
	public static Vec3 operator*(Vec3 vec, double val)
	{ return vec.Multiply(val); }
#endif

	public static Vec3 operator*(Vec3 vec, float val)
	{ return vec.Multiply(val); }
}

%ignore tinyspline::Vec4::operator+;
%ignore tinyspline::Vec4::operator-;
%ignore tinyspline::Vec4::operator*;

%typemap(cscode,noblock=1) tinyspline::Vec4 {
	public static Vec4 operator+(Vec4 a, Vec4 b)
	{ return a.Add(b); }

	public static Vec4 operator-(Vec4 a, Vec4 b)
	{ return a.Subtract(b); }

#ifndef TINYSPLINE_FLOAT_PRECISION
	public static Vec4 operator*(Vec4 vec, double val)
	{ return vec.Multiply(val); }
#endif

	public static Vec4 operator*(Vec4 vec, float val)
	{ return vec.Multiply(val); }
}

%csattributes tinyspline::Vec2::norm
	"[System.ObsoleteAttribute(\"Call Normalize instead\", false)]";
%csattributes tinyspline::Vec3::norm
	"[System.ObsoleteAttribute(\"Call Normalize instead\", false)]";
%csattributes tinyspline::Vec4::norm
	"[System.ObsoleteAttribute(\"Call Normalize instead\", false)]";

%csmethodmodifiers tinyspline::BSpline::toString "public override";
%csmethodmodifiers tinyspline::ChordLengths::toString "public override";
%csmethodmodifiers tinyspline::DeBoorNet::toString "public override";
%csmethodmodifiers tinyspline::Domain::toString "public override";
%csmethodmodifiers tinyspline::Frame::toString "public override";
%csmethodmodifiers tinyspline::FrameSeq::toString "public override";
%csmethodmodifiers tinyspline::Morphism::toString "public override";
%csmethodmodifiers tinyspline::Vec2::toString "public override";
%csmethodmodifiers tinyspline::Vec3::toString "public override";
%csmethodmodifiers tinyspline::Vec4::toString "public override";

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
// std::vector<tinyspline::real> as pointer. As the memory for
// RealVector is always owned by the client, it is safe to create all
// instance with `cMemoryOwn = true` (effectively disabling Swig's
// memory ownership prediction).
%typemap(csout, excode=SWIGEXCODE) std::vector<tinyspline::real> * {
    global::System.IntPtr cPtr = $imcall;
    $csclassname ret = (cPtr == global::System.IntPtr.Zero) ? null : new $csclassname(cPtr, true);$excode
    return ret;
  }

%include "tinyspline.i"

namespace std {
	%template(RealVector) vector<tinyspline::real>;
};
