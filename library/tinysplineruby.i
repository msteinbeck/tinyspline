%module tinysplineruby

//********************************************************
//*                                                      *
//* BSpline (Ruby)                                       *
//*                                                      *
//********************************************************
%ignore ts::BSpline::operator=;

// NOTE: Unfortunately, I was unable to create setter functions for 'ctrlp' and
// 'knots' with SWIG. Thus, 'CMakeLists.txt' creates the file 'tinyspline.rb'
// providing the module 'tinyspline' which 1) requires the module generated
// with SWIG (tinysplineruby) and 2) subclasses BSpline to add the desired
// setters. In order to make the remaining classes (e.g. DeBoorNet) available
// as well, the module 'tinyspline' subclasses them without any further
// implementation simplifying the usage as one needs to import the module
// 'tinyspline' only.

//********************************************************
//*                                                      *
//* DeBoorNet (Ruby)                                     *
//*                                                      *
//********************************************************
%ignore ts::DeBoorNet::operator=;

//********************************************************
//*                                                      *
//* SWIG base file                                       *
//*                                                      *
//********************************************************
%include "tinyspline.i"
