%module tinysplinecsharp

// create a typemap that generalizes the types float and double into a single
// type accessable with: $typemap(cstype, ts::rational)
#ifdef TINYSPLINE_DOUBLE_PRECISION
  %typemap(cstype) ts::rational "double"
#else
  %typemap(cstype) ts::rational "float"
#endif

// let c# interface files use IList<float/double> instead of RationalVector
%typemap(cstype) std::vector<ts::rational>
  "System.Collections.Generic.IList<$typemap(cstype, ts::rational)>"

// redirect input parameter to converter function
%typemap(csin) std::vector<ts::rational>
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

%rename(ctrlp_p) ts::BSpline::ctrlp;
%rename(setCtrlp_p) ts::BSpline::setCtrlp;
%rename(knots_p) ts::BSpline::knots;
%rename(setKnots_p) ts::BSpline::setKnots;

%csmethodmodifiers ts::BSpline::ctrlp "private";
%csmethodmodifiers ts::BSpline::setCtrlp "private";
%csmethodmodifiers ts::BSpline::knots "private";
%csmethodmodifiers ts::BSpline::setKnots "private";

%typemap(cscode) ts::BSpline
%{
  public System.Collections.Generic.IList<$typemap(cstype, ts::rational)> ctrlp {
    get {
      return ctrlp_p();
    }
    set {
      setCtrlp_p(value);
    }
  }

  public System.Collections.Generic.IList<$typemap(cstype, ts::rational)> knots {
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

%typemap(cscode) ts::DeBoorNet
%{
  public System.Collections.Generic.IList<$typemap(cstype, ts::rational)> points {
    get {
      return points_p();
    }
  }

  public System.Collections.Generic.IList<$typemap(cstype, ts::rational)> result {
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
