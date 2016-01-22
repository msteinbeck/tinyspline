%module tinysplinejava

//********************************************************
//*                                                      *
//* TsFloatList (Java)                                   *
//*                                                      *
//********************************************************
%typemap(javaimports) TsFloatList
%{
import java.util.List;
import java.util.AbstractList;
import java.util.RandomAccess;
import java.lang.NullPointerException;
import java.lang.ClassCastException;
%}

%typemap(javabase) TsFloatList "AbstractList<Float>"
%typemap(javainterfaces) TsFloatList "RandomAccess"

%typemap(javacode) TsFloatList
%{
  @Override
  public Float get(final int index) {
    return ts_get(index);
  }

  @Override
  public Float set(final int index, final Float element) {
    if (element == null)
      throw new NullPointerException("Element is null");
    return ts_set(index, element);
  }

  @Override
  public void add(final int index, final Float element) {
    if (element == null)
      throw new NullPointerException("Element is null");
    ts_insert(index, element);
  }

  @Override
  public Float remove(final int index) {
    return ts_remove(index);
  }

  @Override
  public int indexOf(Object o) {
    if (o == null)
      throw new NullPointerException();
    else if (! (o instanceof Float))
      throw new ClassCastException();
    return ts_indexOf((Float) o);
  }

  @Override
  public boolean contains(Object o) {
    return indexOf(o) >= 0;
  }

  @Override
  public int size() {
    return ts_size();
  }

  @Override
  public String toString() {
    return ts_toString();
  }

  static TsFloatVector listToVector(final List<Float> list) {
    if (list == null)
      throw new IllegalArgumentException("The given list must not be null.");

    final TsFloatVector vec = new TsFloatVector();
    for(final float f : list) vec.add(f);
    return vec;
  }
%}

//********************************************************
//*                                                      *
//* TsBSpline (Java)                                     *
//*                                                      *
//********************************************************
%ignore TsBSpline::operator();
%ignore TsBSpline::operator=;

%rename(ctrlp_p) TsBSpline::ctrlp;
%rename(setCtrlp_p) TsBSpline::setCtrlp;
%rename(knots_p) TsBSpline::knots;
%rename(setKnots_p) TsBSpline::setKnots;

%javamethodmodifiers TsBSpline::TsBSpline(const float *points,
    const size_t nPoints, const size_t dim) "private";
%javamethodmodifiers TsBSpline::ctrlp "private";
%javamethodmodifiers TsBSpline::setCtrlp "private";
%javamethodmodifiers TsBSpline::knots "private";
%javamethodmodifiers TsBSpline::setKnots "private";

%typemap(javaimports) TsBSpline
%{
import java.util.List;
%}

%typemap(javacode) TsBSpline
%{
  public TsBSpline(final List<Float> points, final long dim) {
    // implicitly checks null
    this(TsFloatList.listToVector(points), dim);
  }

  public List<Float> getCtrlp() {
    return new TsFloatList(ctrlp_p());
  }

  public void setCtrlp(final List<Float> ctrlp) {
    // implicitly checks null
    setCtrlp_p(TsFloatList.listToVector(ctrlp));
  }

  public List<Float> getKnots() {
    return new TsFloatList(knots_p());
  }

  public void setKnots(final List<Float> knots) {
    // implicitly checks null
    setKnots_p(TsFloatList.listToVector(knots));
  }
%}

//********************************************************
//*                                                      *
//* TsDeBoorNet (Java)                                   *
//*                                                      *
//********************************************************
%ignore TsDeBoorNet::operator=;

%rename(points_p) TsDeBoorNet::points;
%rename(result_p) TsDeBoorNet::result;

%javamethodmodifiers TsDeBoorNet::points "private";
%javamethodmodifiers TsDeBoorNet::result "private";

%typemap(javaimports) TsDeBoorNet 
%{
import java.util.List;
%}

%typemap(javacode) TsDeBoorNet
%{
  public List<Float> getPoints() {
    return new TsFloatList(points_p());
  }

  public List<Float> getResult() {
    return new TsFloatList(result_p());
  }
%}

%include "tinyspline.i"
