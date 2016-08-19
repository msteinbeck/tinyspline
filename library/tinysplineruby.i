%module tinysplineruby

//********************************************************
//*                                                      *
//* BSpline (Ruby)                                       *
//*                                                      *
//********************************************************
%ignore ts::BSpline::operator=;

// NOTE: I was unable to create proper getter and setter functions for 'ctrlp'
// and 'knots' with SWIG. Therefore, a handwritten interface file is provided
// which 1) requires the module generated with SWIG (tinysplineruby), and 2)
// subclasses BSpline and DeBoorNet to add the desired getter and setter
// functions. In order to make the remaining classes (e.g. Utils) available as
// well, the module 'tinyspline' subclasses them without any further
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
