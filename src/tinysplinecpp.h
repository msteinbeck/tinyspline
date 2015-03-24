#pragma once

#include "tinyspline.h"

class TsBSpline {
public:
    TsBSpline();
    TsBSpline(TsBSpline& other);
    TsBSpline(const size_t deg, const size_t dim, const size_t n_ctrlp, 
              const tsBSplineType type);
    ~TsBSpline();
    
    tsBSpline* data();
    float* ctrlp() const;
    float* knots() const;
    
    // getter
    size_t deg() const;
    size_t order() const;
    size_t dim() const;
    size_t nCtrlp() const;
    size_t nKnots() const;
    // setter
    void setDeg(const size_t deg);
    void setOrder(const size_t order);
    void setDim(const size_t dim);
    void setNCtrlp(const size_t nCtrlp);
    void setNKnots(const size_t nKnots);
    
    void setupKnots(const tsBSplineType type);
    size_t insertKnot(const float u, const size_t n);
    void resize(const int n, const int back);
    size_t split(const float u);
    void buckle(const float b);
    void toBesziers();
    
private:
    tsBSpline bspline;
};