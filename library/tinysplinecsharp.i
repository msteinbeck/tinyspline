%module tinysplinecsharp

//********************************************************
//*                                                      *
//* FloatList (C#)                                       *
//*                                                      *
//********************************************************
%typemap(csimports) ts::FloatList
%{
using System;
using System.Collections;
using System.Collections.Generic;
%}

%typemap(csinterfaces) ts::FloatList "IList<float>"

%typemap(cscode) ts::FloatList
%{
  private class TsEnumerator : IEnumerator<float> {
    private FloatList list;
    private int idx;

    public TsEnumerator(FloatList list) {
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

  internal static FloatVector listToVector(IList<float> list) {
    if (list == null)
      throw new ArgumentNullException("The given list must not be null.");

    FloatVector vec = new FloatVector();
    foreach (float f in list) vec.Add(f);
    return vec;
  }
%}

//********************************************************
//*                                                      *
//* BSpline (C#)                                         *
//*                                                      *
//********************************************************
%ignore ts::BSpline::operator();
%ignore ts::BSpline::operator=;

%rename(ctrlp_p) ts::BSpline::ctrlp;
%rename(setCtrlp_p) ts::BSpline::setCtrlp;
%rename(knots_p) ts::BSpline::knots;
%rename(setKnots_p) ts::BSpline::setKnots;

%csmethodmodifiers ts::BSpline::BSpline(const float *points,
    const size_t nPoints, const size_t dim) "private";
%csmethodmodifiers ts::BSpline::ctrlp "private";
%csmethodmodifiers ts::BSpline::setCtrlp "private";
%csmethodmodifiers ts::BSpline::knots "private";
%csmethodmodifiers ts::BSpline::setKnots "private";

%typemap(csimports) ts::BSpline
%{
using System;
using System.Collections;
using System.Collections.Generic;
%}

%typemap(cscode) ts::BSpline
%{
  public BSpline(IList<float> points, uint dim)
      // implicitly checks null
      : this(FloatList.listToVector(points), dim) {}

  public IList<float> ctrlp {
    get {
      return new FloatList(ctrlp_p());
    }
    set {
      // implicitly checks null
      setCtrlp_p(FloatList.listToVector(value));
    }
  }

  public IList<float> knots {
    get {
      return new FloatList(knots_p());
    }
    set {
      // implicitly checks null
      setKnots_p(FloatList.listToVector(value));
    }
  }
%}

//********************************************************
//*                                                      *
//* DeBoorNet (C#)                                       *
//*                                                      *
//********************************************************
%ignore ts::DeBoorNet::operator=;

%rename(points_p) ts::DeBoorNet::points;
%rename(result_p) ts::DeBoorNet::result;

%csmethodmodifiers ts::DeBoorNet::points "private";
%csmethodmodifiers ts::DeBoorNet::result "private";

%typemap(csimports) ts::DeBoorNet
%{
using System.Collections.Generic;
%}

%typemap(cscode) ts::DeBoorNet
%{
  public IList<float> points {
    get {
      return new FloatList(points_p());
    }
  }

  public IList<float> result {
    get {
      return new FloatList(result_p());
    }
  }
%}

%include "tinyspline.i"
