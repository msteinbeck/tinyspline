%module tinysplinepython

%ignore TsDeBoorNet::operator=;
%ignore TsBSpline::operator=;

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
TsBSpline.ctrlpList = None
TsBSpline.knotList = None

def ts_initCtrlpList(self):
    if self.ctrlpList is None:
        self.ctrlpList = TsCtrlpList()
        self.ctrlpList.ts_bspline = self
    return self.ctrlpList

def ts_initKnotList(self):
    if self.knotList is None:
        self.knotList = TsKnotList()
        self.knotList.ts_bspline = self
    return self.knotList

TsBSpline.ctrlp = property(lambda self: ts_initCtrlpList(self))
TsBSpline.knots = property(lambda self: ts_initKnotList(self))
%}

//********************************************************
//*                                                      *
//* TsDeBoorNet (Python)                                 *
//*                                                      *
//********************************************************
%pythoncode %{
TsDeBoorNet.pointList = None
TsDeBoorNet.resultList = None
    
def ts_initPointList(self):
    if self.pointList is None:
        self.pointList = TsPointList()
        self.pointList.ts_deboornet = self
    return self.pointList

def ts_initResultList(self):
    if self.resultList is None:
        self.resultList = TsResultList()
        self.resultList.ts_deboornet = self
    return self.resultList

TsDeBoorNet.points = property(lambda self: ts_initPointList(self))
TsDeBoorNet.result = property(lambda self: ts_initResultList(self))
%}


