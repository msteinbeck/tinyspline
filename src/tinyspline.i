%module tinyspline

// java extensions 
%typemap(javaimports) TsBSpline 
%{
import java.util.AbstractList;
import java.lang.NullPointerException;
import java.util.List;
import java.util.RandomAccess;
%}

%javamethodmodifiers TsBSpline::ctrlp "private";
%javamethodmodifiers TsBSpline::knots "private";

%typemap(javacode) TsBSpline 
%{
  private abstract class TsFloatList extends AbstractList<Float> 
      implements RandomAccess {
    abstract SWIGTYPE_p_float getData();
      
    @Override
    public Float get(int index) {
      return tinyspline.float_array_getitem(getData(), index);
    }
    
    @Override
    public Float set(int index, Float element) {
      if (element == null)
        throw new NullPointerException();
      final Float prev = tinyspline.float_array_getitem(getData(), index);
      tinyspline.float_array_setitem(getData(), index, element);
      return prev;
    }
  }
  
  private final TsFloatList ctrlpList = new TsFloatList() {
    @Override
    SWIGTYPE_p_float getData() {
      return ctrlp();
    }
        
    @Override
    public int size() {
      return (int) getNCtrlp();
    }
  };
  
  private final TsFloatList knotList = new TsFloatList() {
    @Override
    SWIGTYPE_p_float getData() {
      return knots();
    }
        
    @Override
    public int size() {
      return (int) getNKnots();
    }
  };
    
  public List<Float> getCtrlp() {
    return ctrlpList;
  }
  
  public List<Float> getKnots() {
    return knotList;
  }
%}

// make control points and knots accessible
%include <carrays.i>
%array_functions(float, float_array);

// generate getter/setter
%include <attribute.i>
%attribute(TsBSpline, size_t, deg, deg, setDeg);
%attribute(TsBSpline, size_t, order, order, setOrder);
%attribute(TsBSpline, size_t, dim, dim, setDim);
%attribute(TsBSpline, size_t, nCtrlp, nCtrlp);
%attribute(TsBSpline, size_t, nKnots, nKnots);

%ignore tsDeBoorNet;
%ignore tsBSpline;
%ignore TsBSpline::data;

%{
    #include "tinyspline.h"
    #include "tinysplinecpp.h"
%}

%include "tinyspline.h"
%include "tinysplinecpp.h"