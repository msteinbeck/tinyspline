%module tinysplinepython

//********************************************************
//*                                                      *
//* TsFloatList (Python) Part I                          *
//*                                                      *
//********************************************************
%include <pyabc.i>
%pythonabc(TsFloatList, collections.MutableSequence);

//********************************************************
//*                                                      *
//* TsBSpline (Python) Part I                            *
//*                                                      *
//********************************************************
%ignore TsBSpline::operator=;

%feature("pythonprepend") TsBSpline::TsBSpline %{
"""
__init__(self) -> TsBSpline
__init__(self, other) -> TsBSpline
__init__(self, deg, dim, nCtrlp, type) -> TsBSpline
__init__(self, points, dim) -> TsBSpline
"""
if len(args) == 2 and type(args[1]) is int:
    ts_col = args[0]
    ts_dim = args[1]
    if ts_col is None:
        raise ValueError("points must not be None.")
    if ts_dim <= 0:
        raise ValueError("dim must be >= 1.")
    try:
        ts_len = len(ts_col)
        if ts_len % ts_dim != 0:
            raise ValueError("len(points) % dim == 0 failed.")
        ts_arr = new_floatArray(ts_len)
        for i in range(ts_len):
            floatArray_setitem(ts_arr, i, ts_col[i])
        args = (ts_arr, ts_len/ts_dim, ts_dim)
    except TypeError:
        pass
%}

//********************************************************
//*                                                      *
//* TsDeBoorNet (Python) Part I                          *
//*                                                      *
//********************************************************
%ignore TsDeBoorNet::operator=;


// NOTE:
//
// The following code must be included AFTER "tinyspline.i", otherwise the
// classes TsFloatList, TsBSpline and TsDeBoorNet are unknown.
%include "tinyspline.i"


//********************************************************
//*                                                      *
//* TsFloatList (Python) Part II                         *
//*                                                      *
//********************************************************
%pythoncode %{
# http://stackoverflow.com/questions/8294618/define-a-lambda-expression-that-raises-an-exception
def ts_raise(ex):
    raise ex

def ts_indexOf(self, value):
    i = self.ts_indexOf(value)
    if i >= 0:
        return i
    raise ValueError

TsFloatList.__getitem__ = lambda self, index: self.ts_get(index)
TsFloatList.__setitem__ = lambda self, index, value: self.ts_set(index, value)
TsFloatList.index = ts_indexOf
TsFloatList.__contains__ = lambda self, value: self.ts_contains(value)
TsFloatList.__str__ = lambda self: self.ts_toString()
TsFloatList.__len__ = lambda self: self.ts_len()
TsFloatList.__delitem__ = lambda self, index: ts_raise(NotImplementedError("Deleting items is not supported."))
TsFloatList.insert = lambda self, index, value: ts_raise(NotImplementedError("Inserting items is not supported."))
%}

//********************************************************
//*                                                      *
//* TsBSpline (Python) Part II                           *
//*                                                      *
//********************************************************
%pythoncode %{
TsBSpline.ctrlp = property(lambda self: TsCtrlpList(self))
TsBSpline.knots = property(lambda self: TsKnotList(self))
%}

//********************************************************
//*                                                      *
//* TsDeBoorNet (Python) Part II                         *
//*                                                      *
//********************************************************
%pythoncode %{
TsDeBoorNet.points = property(lambda self: TsPointList(self))
TsDeBoorNet.result = property(lambda self: TsResultList(self))
%}


