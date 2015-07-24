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
      if (index < 0)
        throw new ArgumentOutOfRangeException("Negative index: " + index);
      return tinysplinecsharp.float_array_getitem(getSwigPtr(), index);
    }
    set {
      int size = this.Count;
      if (index >= size)
        throw new ArgumentOutOfRangeException("Index: " + index + ", Size: " + size);
      if (index < 0)
        throw new ArgumentOutOfRangeException("Negative index: " + index);
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
%rename(ctrlp_p) TsBSpline::ctrlp;
%csmethodmodifiers TsBSpline::knots "private";
%rename(knots_p) TsBSpline::knots;
%ignore TsBSpline::operator();
%ignore TsBSpline::operator=;

%typemap(csimports) TsBSpline
%{
using System.Collections.Generic;
%}

%typemap(cscode) TsBSpline
%{
  private class TsCtrlpList : TsFloatList {
    internal TsBSpline bspline;

    internal override SWIGTYPE_p_float getSwigPtr() {
      return bspline.ctrlp_p();
    }

    public override int Count {
      get {
        return (int) (bspline.nCtrlp * bspline.dim);
      }
    }
  }

  private class TsKnotList : TsFloatList {
    internal TsBSpline bspline;

    internal override SWIGTYPE_p_float getSwigPtr() {
      return bspline.knots_p();
    }

    public override int Count {
      get {
        return (int)bspline.nKnots;
      }
    }
  }

  TsCtrlpList ctrlpList = new TsCtrlpList();
  TsKnotList knotList = new TsKnotList();

  public IList<float> ctrlp {
    get {
      ctrlpList.bspline = this;
      return ctrlpList;
    }
  }

  public IList<float> knots {
    get {
      knotList.bspline = this;
      return knotList;
    }
  }
%}

//********************************************************
//*                                                      *
//* TsDeBoorNet (C#)                                     *
//*                                                      *
//********************************************************
%csmethodmodifiers TsDeBoorNet::points "private";
%rename(points_p) TsDeBoorNet::points;
%csmethodmodifiers TsDeBoorNet::result "private";
%rename(result_p) TsDeBoorNet::result;
%ignore TsDeBoorNet::operator=;

%typemap(csimports) TsDeBoorNet
%{
using System.Collections.Generic;
%}

%typemap(cscode) TsDeBoorNet
%{
  private class TsPointList : TsFloatList {
    internal TsDeBoorNet deboornet;

    internal override SWIGTYPE_p_float getSwigPtr() {
      return deboornet.points_p();
    }

    public override int Count {
      get {
        return (int) (deboornet.nPoints * deboornet.dim);
      }
    }
  }

  private class TsResultList : TsFloatList {
    internal TsDeBoorNet deboornet;

    internal override SWIGTYPE_p_float getSwigPtr() {
      return deboornet.result_p();
    }

    public override int Count {
      get {
        return (int)deboornet.dim;
      }
    }
  }

  TsPointList pointList = new TsPointList();
  TsResultList resultList = new TsResultList();

  public IList<float> points {
    get {
      pointList.deboornet = this;
      return pointList;
    }
  }

  public IList<float> result {
    get {
      resultList.deboornet = this;
      return resultList;
    }
  }
%}

%include "tinyspline.i"
