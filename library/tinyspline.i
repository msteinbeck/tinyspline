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
%attribute(ts::BSpline, size_t, deg, deg);
%attribute(ts::BSpline, size_t, order, order);
%attribute(ts::BSpline, size_t, dim, dim);
%attribute(ts::BSpline, size_t, nCtrlp, nCtrlp);
%attribute(ts::BSpline, size_t, nKnots, nKnots);
%attributeval(ts::BSpline, std::vector<ts::rational>, ctrlp, ctrlp, setCtrlp);
%attributeval(ts::BSpline, std::vector<ts::rational>, knots, knots, setKnots);
%attribute(ts::DeBoorNet, ts::rational, u, u);
%attribute(ts::DeBoorNet, size_t, k, k);
%attribute(ts::DeBoorNet, size_t, s, s);
%attribute(ts::DeBoorNet, size_t, h, h);
%attribute(ts::DeBoorNet, size_t, dim, dim);
%attribute(ts::DeBoorNet, size_t, nPoints, nPoints);
%attributeval(ts::DeBoorNet, std::vector<ts::rational>, points, points);
%attributeval(ts::DeBoorNet, std::vector<ts::rational>, result, result);

// make strings directly accessible
%include "std_string.i"

// ignore wrapped structs and data fields
%ignore tsError;
%ignore tsDeBoorNet;
%ignore ts::DeBoorNet::data;
%ignore tsBSpline;
%ignore ts::BSpline::data;
// ignore move semantics
%ignore ts::DeBoorNet::DeBoorNet(DeBoorNet &&);
%ignore ts::swap(DeBoorNet &, DeBoorNet &);
%ignore ts::BSpline::BSpline(BSpline &&);
%ignore ts::swap(BSpline &, BSpline &);

%{
    #include "tinyspline.h"
    #include "tinysplinecpp.h"
%}

%include "tinyspline.h"
%include "tinysplinecpp.h"

// make std::vector<ts::rational> available
%include "std_vector.i"
