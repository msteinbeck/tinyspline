%module tinysplinecsharp

// let pinvoke file return IList<float>
%typemap(imtype) std::vector<float> "System.Collections.Generic.IList<float>"

// let c# interface files return IList<float>
%typemap(cstype) std::vector<float> "System.Collections.Generic.IList<float>"

// simply return pinvoke result
%typemap(csout, excode=SWIGEXCODE) std::vector<float> {
  System.Collections.Generic.IList<float> ret = $imcall;$excode
  return ret;
}

// simply redirect the parameter to pinvoke
%typemap(csin) std::vector<float> "$csinput"

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
