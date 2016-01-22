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
      return ts_size();
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
    int count = this.Count;

    if (array == null) {
      throw new ArgumentNullException(
        "The given array must not be null.");
    }
    if (array.Rank > 1) {
      throw new ArgumentException(
        "Multi dimensional arrays are not supported.");
    }
    if (arrayIndex < 0) {
      throw new ArgumentOutOfRangeException(
        "Negative index: " + arrayIndex);
    }
    if (arrayIndex >= array.Length) {
      throw new ArgumentOutOfRangeException(
        "Index: " + arrayIndex + ", Size of array: " + array.Length);
    }
    if (count > array.Length - arrayIndex) {
      throw new ArgumentException(
        "Number of elements to copy is too large.");
    }

    for (int i = 0; i < count; i++, arrayIndex++)
        array[arrayIndex] = ts_get(i);
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
    ts_insert(ts_size(), item);
  }

  public void Insert(int index, float item) {
    ts_insert(index, item);
  }

  public bool Remove(float item) {
    return ts_remove(item);
  }

  public void RemoveAt(int index) {
    ts_remove(index);
  }

  public void Clear() {
    ts_clear();
  }

  public override string ToString() {
    return ts_toString();
  }

  internal static TsFloatVector listToVector(IList<float> list) {
    if (list == null)
      throw new ArgumentNullException("The given list must not be null.");

    TsFloatVector vec = new TsFloatVector();
    foreach (float f in list) vec.Add(f);
    return vec;
  }
%}

//********************************************************
//*                                                      *
//* TsBSpline (C#)                                       *
//*                                                      *
//********************************************************
%ignore TsBSpline::operator();
%ignore TsBSpline::operator=;

%rename(ctrlp_p) TsBSpline::ctrlp;
%rename(setCtrlp_p) TsBSpline::setCtrlp;
%rename(knots_p) TsBSpline::knots;
%rename(setKnots_p) TsBSpline::setKnots;

%csmethodmodifiers TsBSpline::TsBSpline(const float *points,
    const size_t nPoints, const size_t dim) "private";
%csmethodmodifiers TsBSpline::ctrlp "private";
%csmethodmodifiers TsBSpline::setCtrlp "private";
%csmethodmodifiers TsBSpline::knots "private";
%csmethodmodifiers TsBSpline::setKnots "private";

%typemap(csimports) TsBSpline
%{
using System;
using System.Collections;
using System.Collections.Generic;
%}

%typemap(cscode) TsBSpline
%{
  public TsBSpline(IList<float> points, uint dim)
      // implicitly checks null
      : this(TsFloatList.listToVector(points), dim) {}

  public IList<float> ctrlp {
    get {
      return new TsFloatList(ctrlp_p());
    }
    set {
      // implicitly checks null
      setCtrlp_p(TsFloatList.listToVector(value));
    }
  }

  public IList<float> knots {
    get {
      return new TsFloatList(knots_p());
    }
    set {
      // implicitly checks null
      setKnots_p(TsFloatList.listToVector(value));
    }
  }
%}

//********************************************************
//*                                                      *
//* TsDeBoorNet (C#)                                     *
//*                                                      *
//********************************************************
%ignore TsDeBoorNet::operator=;

%rename(points_p) TsDeBoorNet::points;
%rename(result_p) TsDeBoorNet::result;

%csmethodmodifiers TsDeBoorNet::points "private";
%csmethodmodifiers TsDeBoorNet::result "private";

%typemap(csimports) TsDeBoorNet
%{
using System.Collections.Generic;
%}

%typemap(cscode) TsDeBoorNet
%{
  public IList<float> points {
    get {
      return new TsFloatList(points_p());
    }
  }

  public IList<float> result {
    get {
      return new TsFloatList(result_p());
    }
  }
%}

%include "tinyspline.i"
