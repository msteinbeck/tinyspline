%module tinysplinepython

%ignore TsDeBoorNet::operator=;
%ignore TsBSpline::operator=;

%include <pyabc.i>
%pythonabc(TsFloatList, collections.MutableSequence);

%include "tinyspline.i"

// NOTE:
//
// The following code must be included AFTER "tinyspline.i",
// otherwise the classes TsFloatList, TsBSpline and TsDeBoorNet are unknown

//********************************************************
//*                                                      *
//* TsFloatList (Python)                                 *
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
//* TsBSpline (Python)                                   *
//*                                                      *
//********************************************************
%pythoncode %{
TsBSpline.ctrlp = property(lambda self: TsCtrlpList(self))
TsBSpline.knots = property(lambda self: TsCtrlpList(self))
%}

//********************************************************
//*                                                      *
//* TsDeBoorNet (Python)                                 *
//*                                                      *
//********************************************************
%pythoncode %{
TsDeBoorNet.points = property(lambda self: TsPointList(self))
TsDeBoorNet.result = property(lambda self: TsResultList(self))
%}


