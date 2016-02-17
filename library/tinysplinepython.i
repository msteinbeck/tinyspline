%module tinysplinepython

//********************************************************
//*                                                      *
//* BSpline (Python) Part I                              *
//*                                                      *
//********************************************************
%ignore ts::BSpline::operator=;

%rename(__ctrlp) ts::BSpline::ctrlp;
%rename(__setCtrlp) ts::BSpline::setCtrlp;
%rename(__knots) ts::BSpline::knots;
%rename(__setKnots) ts::BSpline::setKnots;

%feature("pythonprepend") ts::BSpline::BSpline %{
"""
__init__(self) -> BSpline
__init__(self, other) -> BSpline
__init__(self, deg, dim, nCtrlp, type) -> BSpline
__init__(self, points, dim) -> BSpline
"""
%}

//********************************************************
//*                                                      *
//* DeBoorNet (Python) Part I                            *
//*                                                      *
//********************************************************
%ignore ts::DeBoorNet::operator=;

%rename(__points) ts::DeBoorNet::points;
%rename(__setPoints) ts::DeBoorNet::setPoints;
%rename(__result) ts::DeBoorNet::result;
%rename(__setResult) ts::DeBoorNet::setResult;


// NOTE:
//
// The following code must be included AFTER "tinyspline.i", otherwise the
// classes FloatList, BSpline and DeBoorNet are unknown.
%include "tinyspline.i"


//********************************************************
//*                                                      *
//* FloatList (Python) Part II                           *
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

FloatList.__getitem__ = lambda self, index: self.ts_get(index)
FloatList.__setitem__ = lambda self, index, value: self.ts_set(index, value)
FloatList.index = ts_indexOf
FloatList.__contains__ = lambda self, value: self.ts_contains(value)
FloatList.__str__ = lambda self: self.ts_toString()
FloatList.__len__ = lambda self: self.ts_size()
FloatList.__delitem__ = lambda self, index: self.ts_remove(index)
FloatList.insert = lambda self, index, value: self.ts_insert(index, value)
%}

//********************************************************
//*                                                      *
//* BSpline (Python) Part II                             *
//*                                                      *
//********************************************************
%pythoncode %{
BSpline.ctrlp = property(lambda self: FloatList(self.__ctrlp()), lambda self, value: self.__setCtrlp(value))
BSpline.knots = property(lambda self: FloatList(self.__knots()), lambda self, value: self.__setKnots(value))
%}

//********************************************************
//*                                                      *
//* DeBoorNet (Python) Part II                           *
//*                                                      *
//********************************************************
%pythoncode %{
DeBoorNet.points = property(lambda self: FloatList(self.__points()), lambda self, value: self.__setPoints(value))
DeBoorNet.result = property(lambda self: FloatList(self.__result()), lambda self, value: self.__setResult(value))
%}


