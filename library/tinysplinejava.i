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
import java.util.RandomAccess;
%}

%typemap(javabase) TsFloatList "AbstractList<Float>"
%typemap(javainterfaces) TsFloatList "RandomAccess"
%typemap(javaclassmodifiers) TsFloatList "public abstract class"

%typemap(javacode) TsFloatList
%{
  abstract SWIGTYPE_p_float getSwigPtr();
      
  @Override
  public Float get(int index) {
    return tinysplinejava.float_array_getitem(getSwigPtr(), index);
  }
    
  @Override
  public Float set(int index, Float element) {
    if (element == null)
      throw new NullPointerException();
    Float prev = tinysplinejava.float_array_getitem(getSwigPtr(), index);
    tinysplinejava.float_array_setitem(getSwigPtr(), index, element);
    return prev;
  }
%}

//********************************************************
//*                                                      *
//* TsBspline (Java)                                     *
//*                                                      *
//********************************************************
%javamethodmodifiers TsBSpline::ctrlp "private";
%javamethodmodifiers TsBSpline::knots "private";
%javamethodmodifiers TsBSpline::operator== "private";
%rename(internalEquals) TsBSpline::operator==;
%ignore TsBSpline::operator();

%typemap(javaimports) TsBSpline
%{
import java.util.List;
%}

%typemap(javacode) TsBSpline
%{
  private final TsFloatList ctrlpList = new TsFloatList() {
    @Override
    SWIGTYPE_p_float getSwigPtr() {
      return ctrlp();
    }
        
    @Override
    public int size() {
      return (int) (getNCtrlp() * getDim());
    }
  };
  
  private final TsFloatList knotList = new TsFloatList() {
    @Override
    SWIGTYPE_p_float getSwigPtr() {
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
  
  @Override
  public boolean equals(Object o) {
    if (o == this)
      return true;
    else if (o instanceof TsBSpline)
      return internalEquals((TsBSpline) o);
    else
      return false;
  }
%}

//********************************************************
//*                                                      *
//* TsDeboorNet (Java)                                   *
//*                                                      *
//********************************************************
%javamethodmodifiers TsDeBoorNet::points "private";
%javamethodmodifiers TsDeBoorNet::result "private";

%typemap(javaimports) TsDeBoorNet 
%{
import java.util.List;
%}

%typemap(javacode) TsDeBoorNet
%{
  private final TsFloatList pointsList = new TsFloatList() {
    @Override
    SWIGTYPE_p_float getSwigPtr() {
      return points();
    }
        
    @Override
    public int size() {
      return (int) (getNPoints() * getDim());
    }
  };
  
  private final TsFloatList resultList = new TsFloatList() {
    @Override
    SWIGTYPE_p_float getSwigPtr() {
      return result();
    }
    
    @Override
    public int size() {
      return (int) getDim();
    }
  };
    
  public List<Float> getPoints() {
    return pointsList;
  }
  
  public List<Float> getResult() {
    return resultList;
  }
%}


%include "tinyspline.i"
