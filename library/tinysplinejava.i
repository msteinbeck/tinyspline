%module tinysplinejava

//********************************************************
//*                                                      *
//* FloatList (Java)                                     *
//*                                                      *
//********************************************************
%typemap(javaimports) ts::FloatList
%{
import java.util.List;
import java.util.AbstractList;
import java.util.RandomAccess;
import java.lang.NullPointerException;
import java.lang.ClassCastException;
%}

%typemap(javabase) ts::FloatList "AbstractList<Float>"
%typemap(javainterfaces) ts::FloatList "RandomAccess"

%typemap(javacode) ts::FloatList
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

  static FloatVector listToVector(final List<Float> list) {
    if (list == null)
      throw new IllegalArgumentException("The given list must not be null.");

    final FloatVector vec = new FloatVector();
    for(final float f : list) vec.add(f);
    return vec;
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
    // implicitly checks null
    this(FloatList.listToVector(points), dim);
  }

  public List<Float> getCtrlp() {
    return new FloatList(ctrlp_p());
  }

  public void setCtrlp(final List<Float> ctrlp) {
    // implicitly checks null
    setCtrlp_p(FloatList.listToVector(ctrlp));
  }

  public List<Float> getKnots() {
    return new FloatList(knots_p());
  }

  public void setKnots(final List<Float> knots) {
    // implicitly checks null
    setKnots_p(FloatList.listToVector(knots));
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
    return new FloatList(points_p());
  }

  public List<Float> getResult() {
    return new FloatList(result_p());
  }
%}

%include "tinyspline.i"
