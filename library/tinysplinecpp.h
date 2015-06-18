#pragma once

#include "tinyspline.h"

class TsDeBoorNet {
public:
    TsDeBoorNet();
    TsDeBoorNet(const TsDeBoorNet& other);
    ~TsDeBoorNet();
    
    tsDeBoorNet* data();
    float* points() const;
    float* result() const;
    
    float u() const;
    size_t k() const;
    size_t s() const;
    size_t h() const;
    size_t dim() const;
    size_t nPoints() const;
    
private:
    tsDeBoorNet deBoorNet;
};

class TsBSpline {
public:
    TsBSpline();
    TsBSpline(const TsBSpline& other);
    TsBSpline(const size_t deg, const size_t dim, const size_t n_ctrlp, 
              const tsBSplineType type);
    ~TsBSpline();
    
    bool operator==(const TsBSpline& other) const;
    TsDeBoorNet operator()(const float u) const;
    
    tsBSpline* data();
    float* ctrlp() const;
    float* knots() const;
    
    size_t deg() const;
    size_t order() const;
    size_t dim() const;
    size_t nCtrlp() const;
    size_t nKnots() const;
    
    void setDeg(const size_t deg);
    void setOrder(const size_t order);
    
    void setupKnots(const tsBSplineType type);
    void resize(const int n, const int back);
    void buckle(const float b);
    void toBeziers();
    size_t insertKnot(const float u, const size_t n);
    size_t split(const float u);
    TsDeBoorNet evaluate(const float u) const;
    
private:
    tsBSpline bspline;
};