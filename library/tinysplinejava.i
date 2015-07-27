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
%javamethodmodifiers TsBSpline::ctrlp "private";
%javamethodmodifiers TsBSpline::knots "private";
%ignore TsBSpline::operator();
%ignore TsBSpline::operator=;

%typemap(javaimports) TsBSpline
%{
import java.util.List;
%}

%typemap(javacode) TsBSpline
%{
  private final TsFloatList ctrlpList = new TsCtrlpList();
  private final TsFloatList knotList = new TsKnotList();

  public List<Float> getCtrlp() {
    ctrlpList.setTs_bspline(this);
    return ctrlpList;
  }

  public List<Float> getKnots() {
    knotList.setTs_bspline(this);
    return knotList;
  }
%}

//********************************************************
//*                                                      *
//* TsDeBoorNet (Java)                                   *
//*                                                      *
//********************************************************
%javamethodmodifiers TsDeBoorNet::points "private";
%javamethodmodifiers TsDeBoorNet::result "private";
%ignore TsDeBoorNet::operator=;

%typemap(javaimports) TsDeBoorNet 
%{
import java.util.List;
%}

%typemap(javacode) TsDeBoorNet
%{
  private final TsFloatList pointList = new TsPointList();
  private final TsFloatList resultList = new TsResultList();

  public List<Float> getPoints() {
    pointList.setTs_deboornet(this);
    return pointList;
  }

  public List<Float> getResult() {
    resultList.setTs_deboornet(this);
    return resultList;
  }
%}


%include "tinyspline.i"
