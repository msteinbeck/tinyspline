%module tinysplinepython

// std::vector<ts::rational> to python list
%typemap(out) std::vector<ts::rational> * (int size, PyObject * data) {
  size = $1->size();
  $result = PyList_New(size);
  for (int i = 0; i < size; i++) {
    data = PyFloat_FromDouble((*$1)[i]);
    PyList_SetItem($result, i, data);
  }
}

// python list to std::vector<ts::rational>
%typemap(in) std::vector<ts::rational> * (int size, PyObject * data) %{
  size = PyList_Size($input);
  $1 = new std::vector<ts::rational>();
  $1->reserve(size);
  for (int i = 0; i < size; i++) {
    data = PyList_GetItem($input, i);
    $1->push_back(PyFloat_AsDouble(data));
  }
%}

// cleanup memory allocated by typemap
%typemap(freearg) std::vector<ts::rational> * {
  delete $1;
}

//********************************************************
//*                                                      *
//* BSpline (Python)                                     *
//*                                                      *
//********************************************************
%ignore ts::BSpline::operator=;

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

//********************************************************
//*                                                      *
//* SWIG base file                                       *
//*                                                      *
//********************************************************
%include "tinyspline.i"
