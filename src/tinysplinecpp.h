#pragma once

#include "tinyspline.h"

class TsBSpline {
public:
    TsBSpline();
    TsBSpline(TsBSpline& other);
    TsBSpline(const size_t deg, const size_t dim, const size_t n_ctrlp, const tsBSplineType type);
    ~TsBSpline();
    
    tsBSpline* data();
    
    size_t deg();
    size_t order();
    size_t dim();
    size_t nCtrlp();
    size_t nKnots();
    float* ctrlp();
    float* konts();
    
private:
    tsBSpline bspline;
};