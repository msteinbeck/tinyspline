%module tinysplinejava

//********************************************************
//*                                                      *
//* TsFloatList (Java)                                   *
//*                                                      *
//********************************************************
%typemap(javaimports) TsFloatList
%{
import java.util.AbstractList;
import java.lang.NullPointerException;
import java.lang.RuntimeException;
import java.util.RandomAccess;
%}

%typemap(javabase) TsFloatList "AbstractList<Float>"
%typemap(javainterfaces) TsFloatList "RandomAccess"
%typemap(javaclassmodifiers) TsFloatList "public abstract class"

%typemap(javacode) TsFloatList
%{
  @Override
  public Float get(int index) {
    return ts_get(index);
  }
    
  @Override
  public Float set(int index, Float element) {
    if (element == null)
      throw new NullPointerException();
    return ts_set(index, element);
  }

  @Override
  public int size() {
    return ts_len();
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
%}

//********************************************************
//*                                                      *
//* TsBSpline (Java)                                     *
//*                                                      *
//********************************************************
%ignore TsBSpline::operator();
%ignore TsBSpline::operator=;

%javamethodmodifiers TsBSpline::TsBSpline(const float *points,
    const size_t nPoints, const size_t dim) "private";

%typemap(javaimports) TsBSpline
%{
import java.util.List;
import java.lang.IllegalArgumentException;
%}

%typemap(javacode) TsBSpline
%{
  private static SWIGTYPE_p_float prepareInterpolation(
      final List<Float> points, final long dim) {
    if (points == null) {
      throw new IllegalArgumentException("points must not be null.");
    } else if (dim <= 0) {
      throw new IllegalArgumentException("dim must be >= 1.");
    }

    final int size = points.size();
    final SWIGTYPE_p_float array = tinysplinejava.new_floatArray(size);
    for (int i = 0; i < size; i++) {
      tinysplinejava.floatArray_setitem(array, i, points.get(i));
    }

    return array;
  }

  public TsBSpline(final List<Float> points, final long dim) {
    this(prepareInterpolation(points, dim), points.size(), dim);
  }

  public List<Float> getCtrlp() {
    return new TsCtrlpList(this);
  }

  public List<Float> getKnots() {
    return new TsKnotList(this);
  }
%}

//********************************************************
//*                                                      *
//* TsDeBoorNet (Java)                                   *
//*                                                      *
//********************************************************
%ignore TsDeBoorNet::operator=;

%typemap(javaimports) TsDeBoorNet 
%{
import java.util.List;
%}

%typemap(javacode) TsDeBoorNet
%{
  public List<Float> getPoints() {
    return new TsPointList(this);
  }

  public List<Float> getResult() {
    return new TsResultList(this);
  }
%}

%include "tinyspline.i"
