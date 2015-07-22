%module tinysplinecsharp

//********************************************************
//*                                                      *
//* TsFloatList (C#)                                     *
//*                                                      *
//********************************************************
%typemap(csimports) TsFloatList
%{
using System;
using System.Collections;
using System.Collections.Generic;
%}

%typemap(csinterfaces) TsFloatList "IList<float>"
%typemap(csclassmodifiers) TsFloatList "public abstract class"

%typemap(cscode) TsFloatList
%{
  private class TsEnumerator : IEnumerator<float> {
    private TsFloatList list;
    private int idx;

    public TsEnumerator(TsFloatList list) {
      this.list = list;
      this.idx = 0;
    }

    public float Current {
      get {
        return tinysplinecsharp.float_array_getitem(list.getSwigPtr(), idx);
      }
    }

    object IEnumerator.Current {
      get {
        return this.Current;
      }
    }

    public bool MoveNext() {
      return ++idx >= list.Count;
    }

    public void Reset() {
      idx = 0;
    }

    public void Dispose() {}
  }

  internal abstract SWIGTYPE_p_float getSwigPtr();
  public abstract int Count { get; }

  public bool IsReadOnly {
    get {
      return false;
    }
  }

  public float this[int index] {
    get {
      int size = this.Count;
      if (index >= size)
        throw new ArgumentOutOfRangeException("Index: " + index + ", Size: " + size);
      return tinysplinecsharp.float_array_getitem(getSwigPtr(), index);
    }
    set {
      int size = this.Count;
      if (index >= size)
        throw new ArgumentOutOfRangeException("Index: " + index + ", Size: " + size);
      tinysplinecsharp.float_array_setitem(getSwigPtr(), index, value);
    }
  }

  public void CopyTo(float[] array, int arrayIndex) {
    if (array == null)
      throw new ArgumentNullException();
    if (arrayIndex < 0)
      throw new ArgumentOutOfRangeException("Index: " + arrayIndex);
    int size = this.Count;
    if (array.Length < size)
      throw new ArgumentException("Array: " + array.Length + ", List: " + size);
    for (; arrayIndex < size; arrayIndex++)
      array[arrayIndex] = tinysplinecsharp.float_array_getitem(getSwigPtr(), arrayIndex);
  }

  public int IndexOf(float item) {
    int size = this.Count;
    for (int i = 0; i < size; i++) {
      if (item == tinysplinecsharp.float_array_getitem(getSwigPtr(), i))
        return i;
    }
    return -1;
  }

  public bool Contains(float item) {
    return IndexOf(item) >= 0;
  }

  public IEnumerator<float> GetEnumerator() {
    return new TsEnumerator(this);
  }

  IEnumerator IEnumerable.GetEnumerator() {
    return GetEnumerator();
  }

  public void Add(float item) {
    throw new NotSupportedException("Adding items is not supported.");
  }

  public void Insert(int index, float item) {
    throw new NotSupportedException("Inserting items is not supported.");
  }

  public bool Remove(float item) {
    throw new NotSupportedException("Removing items is not supported.");
  }

  public void RemoveAt(int index) {
    throw new NotSupportedException("Removing items is not supported.");
  }

  public void Clear() {
    throw new NotSupportedException("Removing items is not supported.");
  }

  public override string ToString() {
    string s = "[";
    int lst = this.Count - 1;
    for (int i = 0; i < lst; i++) {
      s += tinysplinecsharp.float_array_getitem(getSwigPtr(), i);
      s += ", ";
    }
    s += tinysplinecsharp.float_array_getitem(getSwigPtr(), lst);
    s += "]";
    return s;
  }
%}

//********************************************************
//*                                                      *
//* TsBSpline (C#)                                       *
//*                                                      *
//********************************************************
%csmethodmodifiers TsBSpline::ctrlp "private";
%csmethodmodifiers TsBSpline::knots "private";
%ignore TsBSpline::operator();
%ignore TsBSpline::operator=;
%csmethodmodifiers TsBSpline::operator== "private";
%rename(internalEquals) TsBSpline::operator==;

%typemap(csimports) TsBSpline
%{
using System.Collections.Generic;
%}

%typemap(cscode) TsBSpline
%{
  private class TsCtrlpList : TsFloatList {
    private TsBSpline bspline;

    public TsCtrlpList(TsBSpline bspline) {
      this.bspline = bspline;
    }

    internal override SWIGTYPE_p_float getSwigPtr() {
      return bspline.ctrlp();
    }

    public override int Count {
      get {
        return (int) (bspline.nCtrlp * bspline.dim);
      }
    }
  }

  private class TsKnotList : TsFloatList {
    private TsBSpline bspline;

    public TsKnotList(TsBSpline bspline) {
      this.bspline = bspline;
    }

    internal override SWIGTYPE_p_float getSwigPtr() {
      return bspline.knots();
    }

    public override int Count {
      get {
        return (int)bspline.nKnots;
      }
    }
  }

  public IList<float> getCtrlp() {
    return new TsCtrlpList(this);
  }

  public IList<float> getKnots() {
    return new TsKnotList(this);
  }

  public static bool operator== (TsBSpline b1, TsBSpline b2) {
    return b1.internalEquals(b2);
  }

  public static bool operator!= (TsBSpline b1, TsBSpline b2) {
    return !b1.internalEquals(b2);
  }

  public override bool Equals(object o) {
    if (o == null) {
      return false;
    } else if (o is TsBSpline) {
      return internalEquals((TsBSpline)o);
    } else {
      return false;
    }
  }

  // suppress compiler warning about Equals and GetHashCode
  public override int GetHashCode() {
    return base.GetHashCode();
  }
%}

//********************************************************
//*                                                      *
//* TsDeBoorNet (C#)                                     *
//*                                                      *
//********************************************************
%csmethodmodifiers TsDeBoorNet::points "private";
%csmethodmodifiers TsDeBoorNet::result "private";
%ignore TsDeBoorNet::operator=;

%typemap(csimports) TsDeBoorNet
%{
using System.Collections.Generic;
%}

%typemap(cscode) TsDeBoorNet
%{
  private class TsPointList : TsFloatList {
    private TsDeBoorNet deboornet;

    public TsPointList(TsDeBoorNet deboornet) {
      this.deboornet = deboornet;
    }

    internal override SWIGTYPE_p_float getSwigPtr() {
      return deboornet.points();
    }

    public override int Count {
      get {
        return (int) (deboornet.nPoints * deboornet.dim);
      }
    }
  }

  private class TsResultList : TsFloatList {
    private TsDeBoorNet deboornet;

    public TsResultList(TsDeBoorNet deboornet) {
      this.deboornet = deboornet;
    }

    internal override SWIGTYPE_p_float getSwigPtr() {
      return deboornet.result();
    }

    public override int Count {
      get {
        return (int)deboornet.dim;
      }
    }
  }

  public IList<float> getPoints() {
    return new TsPointList(this);
  }

  public IList<float> getResult() {
    return new TsResultList(this);
  }
%}

%include "tinyspline.i"
