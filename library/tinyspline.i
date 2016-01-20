// NOTE:
//
// This file configures all language independent settings. It gets 
// included in each concrete module.
//
// For some reason you MUST include this file AFTER typemaps.
// Use %include "tinyspline.i" to include this file.

// exception handling
%include "exception.i"
%exception {
  try {
    $action
  } catch (const std::out_of_range& e) {
    SWIG_exception(SWIG_IndexError, e.what());
  } catch (const std::runtime_error& e) {
    SWIG_exception(SWIG_RuntimeError, e.what());
  }
}

// generate getter/setter
%include <attribute.i>
%attribute(TsBSpline, size_t, deg, deg, setDeg);
%attribute(TsBSpline, size_t, order, order, setOrder);
%attribute(TsBSpline, size_t, dim, dim);
%attribute(TsBSpline, size_t, nCtrlp, nCtrlp);
%attribute(TsBSpline, size_t, nKnots, nKnots);
%attribute(TsDeBoorNet, float, u, u);
%attribute(TsDeBoorNet, size_t, k, k);
%attribute(TsDeBoorNet, size_t, s, s);
%attribute(TsDeBoorNet, size_t, h, h);
%attribute(TsDeBoorNet, size_t, dim, dim);
%attribute(TsDeBoorNet, size_t, nPoints, nPoints);

// make strings directly accessible
%include "std_string.i"

// make std::vector<float> available
%include "std_vector.i"
namespace std {
  %template(TsFloatVector) vector<float>;
};

// ignore wrapped structs and data fields
%ignore tsError;
%ignore tsDeBoorNet;
%ignore TsDeBoorNet::data;
%ignore tsBSpline;
%ignore TsBSpline::data;

%{
    #include "tinyspline.h"
    #include "tinysplinecpp.h"
    #include "swig_wrapper.h"
%}

%include "tinyspline.h"
%include "tinysplinecpp.h"
%include "swig_wrapper.h"
