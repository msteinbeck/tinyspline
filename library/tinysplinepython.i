%module tinysplinepython

//********************************************************
//*                                                      *
//* TsBSpline (Python) Part I                            *
//*                                                      *
//********************************************************
%ignore TsBSpline::operator=;

%rename(__ctrlp) TsBSpline::ctrlp;
%rename(__setCtrlp) TsBSpline::setCtrlp;
%rename(__knots) TsBSpline::knots;
%rename(__setKnots) TsBSpline::setKnots;

%feature("pythonprepend") TsBSpline::TsBSpline %{
"""
__init__(self) -> TsBSpline
__init__(self, other) -> TsBSpline
__init__(self, deg, dim, nCtrlp, type) -> TsBSpline
__init__(self, points, dim) -> TsBSpline
"""
%}

//********************************************************
//*                                                      *
//* TsDeBoorNet (Python) Part I                          *
//*                                                      *
//********************************************************
%ignore TsDeBoorNet::operator=;

%rename(__points) TsDeBoorNet::points;
%rename(__setPoints) TsDeBoorNet::setPoints;
%rename(__result) TsDeBoorNet::result;
%rename(__setResult) TsDeBoorNet::setResult;


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
TsFloatList.__len__ = lambda self: self.ts_size()
TsFloatList.__delitem__ = lambda self, index: self.ts_remove(index)
TsFloatList.insert = lambda self, index, value: self.ts_insert(index, value)
%}

//********************************************************
//*                                                      *
//* TsBSpline (Python) Part II                           *
//*                                                      *
//********************************************************
%pythoncode %{
TsBSpline.ctrlp = property(lambda self: TsFloatList(self.__ctrlp()), lambda self, value: self.__setCtrlp(value))
TsBSpline.knots = property(lambda self: TsFloatList(self.__knots()), lambda self, value: self.__setKnots(value))
%}

//********************************************************
//*                                                      *
//* TsDeBoorNet (Python) Part II                         *
//*                                                      *
//********************************************************
%pythoncode %{
TsDeBoorNet.points = property(lambda self: TsFloatList(self.__points()), lambda self, value: self.__setPoints(value))
TsDeBoorNet.result = property(lambda self: TsFloatList(self.__result()), lambda self, value: self.__setResult(value))
%}


