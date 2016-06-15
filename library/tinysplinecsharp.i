%module tinysplinecsharp

%typemap(cstype) std::vector<float> "System.Collections.Generic.IList<float>"

%typemap(csin) std::vector<float> "FloatVector.getCPtr(new FloatVector((System.Collections.ICollection)$csinput))"

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
