%module tinysplinepython

//********************************************************
//*                                                      *
//* BSpline (Python)                                     *
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
//* DeBoorNet (Python)                                   *
//*                                                      *
//********************************************************
%ignore ts::DeBoorNet::operator=;

%rename(__points) ts::DeBoorNet::points;
%rename(__result) ts::DeBoorNet::result;

//********************************************************
//*                                                      *
//* SWIG base file                                       *
//*                                                      *
//********************************************************
%include "tinyspline.i"

// NOTE: The following code must be included AFTER "tinyspline.i".
%pythoncode %{
  BSpline.ctrlp = property(lambda self: list(self.__ctrlp()), lambda self, value: self.__setCtrlp(RationalVector(value)))
  BSpline.knots = property(lambda self: list(self.__knots()), lambda self, value: self.__setKnots(RationalVector(value)))
  DeBoorNet.points = property(lambda self: list(self.__points()))
  DeBoorNet.result = property(lambda self: list(self.__result()))
%}
