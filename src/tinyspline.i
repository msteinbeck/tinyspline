%module tinyspline

%include <carrays.i>
%array_functions(float, float_array);

%include <attribute.i>
%attribute(TsBSpline, size_t, deg, deg, setDeg);
%attribute(TsBSpline, size_t, order, order, setOrder);
%attribute(TsBSpline, size_t, dim, dim, setDim);
%attribute(TsBSpline, size_t, nCtrlp, nCtrlp, setNCtrlp);
%attribute(TsBSpline, size_t, nKnots, nKnots, setNKnots);

%{
    #include "tinyspline.h"
    #include "tinysplinecpp.h"
%}

%ignore tsDeBoorNet;
%ignore tsBSpline;

%include "tinyspline.h"
%include "tinysplinecpp.h"
