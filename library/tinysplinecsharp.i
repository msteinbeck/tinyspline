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
        return list.ts_get(idx);
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

  public int Count {
    get {
      return ts_len();
    }
  }

  public bool IsReadOnly {
    get {
      return false;
    }
  }

  public float this[int index] {
    get {
      return ts_get(index);
    }
    set {
      ts_set(index, value);
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
      array[arrayIndex] = ts_get(arrayIndex);
  }

  public int IndexOf(float item) {
    return ts_indexOf(item);
  }

  public bool Contains(float item) {
    return ts_contains(item);
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
    return ts_toString();
  }
%}

//********************************************************
//*                                                      *
//* TsBSpline (C#)                                       *
//*                                                      *
//********************************************************
%ignore TsBSpline::operator();
%ignore TsBSpline::operator=;

%csmethodmodifiers TsBSpline::TsBSpline(const float *points,
    const size_t nPoints, const size_t dim) "private";

%typemap(csimports) TsBSpline
%{
using System;
using System.Collections;
using System.Collections.Generic;
%}

%typemap(cscode) TsBSpline
%{
  private static SWIGTYPE_p_float prepareInterpolation(
      IList<float> points, uint dim) {
    if (points == null) {
      throw new ArgumentNullException();
    }

    int count = points.Count;
    SWIGTYPE_p_float array = tinysplinecsharp.new_floatArray(count);
    for (int i = 0; i < count; i++) {
      tinysplinecsharp.floatArray_setitem(array, i, points[i]);
    }

    return array;
  }

  public TsBSpline(IList<float> points, uint dim)
      : this(prepareInterpolation(points, dim), (uint)points.Count, dim) {}

  public IList<float> ctrlp {
    get {
      return new TsCtrlpList(this);
    }
  }

  public IList<float> knots {
    get {
      return new TsKnotList(this);
    }
  }
%}

//********************************************************
//*                                                      *
//* TsDeBoorNet (C#)                                     *
//*                                                      *
//********************************************************
%ignore TsDeBoorNet::operator=;

%typemap(csimports) TsDeBoorNet
%{
using System.Collections.Generic;
%}

%typemap(cscode) TsDeBoorNet
%{
  public IList<float> points {
    get {
      return new TsPointList(this);
    }
  }

  public IList<float> result {
    get {
      return new TsResultList(this);
    }
  }
%}

%include "tinyspline.i"
