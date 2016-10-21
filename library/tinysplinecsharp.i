%module tinysplinecsharp

// create a typemap that generalizes the types float and double to a single
// type accessable with: $typemap(cstype, ts::rational)
#ifdef TINYSPLINE_DOUBLE_PRECISION
  %typemap(cstype) ts::rational "double"
#else
  %typemap(cstype) ts::rational "float"
#endif

// change the signature of the c# interface files from
// std::vector<ts::rational> to IList<float/double>
%typemap(cstype) std::vector<ts::rational> *
  "System.Collections.Generic.IList<$typemap(cstype, ts::rational)>"

// let c# interface files redirect the input argument to the converter
// function
%typemap(csin) std::vector<ts::rational> *
  "RationalVector.getCPtr(Utils.ListToVector($csinput))"

//********************************************************
//*                                                      *
//* Utils (C#)                                           *
//*                                                      *
//********************************************************
%typemap(cscode) ts::Utils
%{
  internal static RationalVector ListToVector(
      System.Collections.Generic.IList<$typemap(cstype, ts::rational)> list) {
    if (list == null)
      throw new System.ArgumentNullException("List must not be null.");

    RationalVector vec = new RationalVector(list.Count);
    foreach ($typemap(cstype, ts::rational) val in list) vec.Add(val);
    return vec;
  }
%}

//********************************************************
//*                                                      *
//* BSpline (C#)                                         *
//*                                                      *
//********************************************************
%ignore ts::BSpline::operator();
%ignore ts::BSpline::operator=;

//********************************************************
//*                                                      *
//* DeBoorNet (C#)                                       *
//*                                                      *
//********************************************************
%ignore ts::DeBoorNet::operator=;

//********************************************************
//*                                                      *
//* SWIG base file                                       *
//*                                                      *
//********************************************************
%include "tinyspline.i"

namespace std {
    %template(RationalVector) vector<ts::rational>;
};
