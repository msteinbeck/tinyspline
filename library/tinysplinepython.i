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
if len(args) == 3 and \
        type(args[1]) is int and \
        type(args[2]) is int:
    try:
        ts_col = args[0]
        ts_len = len(ts_col)
        ts_arr = new_floatArray(ts_len)
        for i in range(ts_len):
            floatArray_setitem(ts_arr, i, ts_col[i])
        args = (ts_arr, args[1], args[2])
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
TsBSpline.knots = property(lambda self: TsCtrlpList(self))
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


