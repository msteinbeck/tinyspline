%module tinysplinecsharp

%typemap(cstype) std::vector<float> "System.Collections.Generic.IList<float>"
%typemap(csin) std::vector<float> "FloatVector.getCPtr(Utils.ListToVector($csinput))"

//********************************************************
//*                                                      *
//* Utils (C#)                                           *
//*                                                      *
//********************************************************
%typemap(csimports) ts::Utils
%{
using System;
using System.Collections;
using System.Collections.Generic;
%}

%typemap(cscode) ts::Utils
%{
  internal static FloatVector ListToVector(IList<float> list) {
    if (list == null)
      throw new ArgumentNullException("The given list must not be null.");

    FloatVector vec = new FloatVector(list.Count);
    foreach (float f in list) vec.Add(f);
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

%rename(ctrlp_p) ts::BSpline::ctrlp;
%rename(setCtrlp_p) ts::BSpline::setCtrlp;
%rename(knots_p) ts::BSpline::knots;
%rename(setKnots_p) ts::BSpline::setKnots;

%csmethodmodifiers ts::BSpline::ctrlp "private";
%csmethodmodifiers ts::BSpline::setCtrlp "private";
%csmethodmodifiers ts::BSpline::knots "private";
%csmethodmodifiers ts::BSpline::setKnots "private";

%typemap(csimports) ts::BSpline
%{
using System;
using System.Collections;
using System.Collections.Generic;
%}

%typemap(cscode) ts::BSpline
%{
  public IList<float> ctrlp {
    get {
      return ctrlp_p();
    }
    set {
      setCtrlp_p(value);
    }
  }

  public IList<float> knots {
    get {
      return knots_p();
    }
    set {
      setKnots_p(value);
    }
  }
%}

//********************************************************
//*                                                      *
//* DeBoorNet (C#)                                       *
//*                                                      *
//********************************************************
%ignore ts::DeBoorNet::operator=;

%rename(points_p) ts::DeBoorNet::points;
%rename(result_p) ts::DeBoorNet::result;

%csmethodmodifiers ts::DeBoorNet::points "private";
%csmethodmodifiers ts::DeBoorNet::result "private";

%typemap(csimports) ts::DeBoorNet
%{
using System.Collections.Generic;
%}

%typemap(cscode) ts::DeBoorNet
%{
  public IList<float> points {
    get {
      return points_p();
    }
  }

  public IList<float> result {
    get {
      return result_p();
    }
  }
%}

//********************************************************
//*                                                      *
//* SWIG base file                                       *
//*                                                      *
//********************************************************
%include "tinyspline.i"
