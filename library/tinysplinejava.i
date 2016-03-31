%module tinysplinejava

//********************************************************
//*                                                      *
//* Utils (Java)                                         *
//*                                                      *
//********************************************************
%typemap(javaimports) ts::Utils
%{
import java.util.List;
import java.util.ArrayList;
import java.lang.Integer;
import java.lang.IllegalArgumentException;
%}

%typemap(javacode) ts::Utils
%{
  static FloatVector listToVector(final List<Float> list) {
    if (list == null)
      throw new IllegalArgumentException("The given list must not be null.");

    final FloatVector vec = new FloatVector(list.size());
    for(final float f : list) vec.add(f);
    return vec;
  }

  static List<Float> vectorToList(final FloatVector vec) {
    if (vec == null) {
      throw new IllegalArgumentException("The given vector must not be null.");
    }
    final long size = vec.size();
    if (size > (long)Integer.MAX_VALUE) {
      throw new IllegalArgumentException(
        "Unable to store " + size + " many float in a list.");
    }

    final List<Float> list = new ArrayList<Float>((int)size);
    for (int i = 0; i < size; i++) list.add(vec.get(i));
    return list;
  }
%}

//********************************************************
//*                                                      *
//* BSpline (Java)                                       *
//*                                                      *
//********************************************************
%ignore ts::BSpline::operator();
%ignore ts::BSpline::operator=;

%rename(ctrlp_p) ts::BSpline::ctrlp;
%rename(setCtrlp_p) ts::BSpline::setCtrlp;
%rename(knots_p) ts::BSpline::knots;
%rename(setKnots_p) ts::BSpline::setKnots;

%javamethodmodifiers ts::BSpline::BSpline(const float *points,
    const size_t nPoints, const size_t dim) "private";
%javamethodmodifiers ts::BSpline::ctrlp "private";
%javamethodmodifiers ts::BSpline::setCtrlp "private";
%javamethodmodifiers ts::BSpline::knots "private";
%javamethodmodifiers ts::BSpline::setKnots "private";

%typemap(javaimports) ts::BSpline
%{
import java.util.List;
%}

%typemap(javacode) ts::BSpline
%{
  public BSpline(final List<Float> points, final long dim) {
    this(Utils.listToVector(points), dim);
  }

  public List<Float> getCtrlp() {
    return Utils.vectorToList(ctrlp_p());
  }

  public void setCtrlp(final List<Float> ctrlp) {
    setCtrlp_p(Utils.listToVector(ctrlp));
  }

  public List<Float> getKnots() {
    return Utils.vectorToList(knots_p());
  }

  public void setKnots(final List<Float> knots) {
    setKnots_p(Utils.listToVector(knots));
  }
%}

//********************************************************
//*                                                      *
//* DeBoorNet (Java)                                     *
//*                                                      *
//********************************************************
%ignore ts::DeBoorNet::operator=;

%rename(points_p) ts::DeBoorNet::points;
%rename(result_p) ts::DeBoorNet::result;

%javamethodmodifiers ts::DeBoorNet::points "private";
%javamethodmodifiers ts::DeBoorNet::result "private";

%typemap(javaimports) ts::DeBoorNet
%{
import java.util.List;
%}

%typemap(javacode) ts::DeBoorNet
%{
  public List<Float> getPoints() {
    return Utils.vectorToList(points_p());
  }

  public List<Float> getResult() {
    return Utils.vectorToList(result_p());
  }
%}

//********************************************************
//*                                                      *
//* SWIG base file                                       *
//*                                                      *
//********************************************************
%include "tinyspline.i"
