#pragma once

#include "tinyspline.h"

class TsDeBoorNet {
public:
    TsDeBoorNet();
    TsDeBoorNet(const TsDeBoorNet& other);
    ~TsDeBoorNet();

    TsDeBoorNet& operator=(const TsDeBoorNet& other);

    float u() const;
    size_t k() const;
    size_t s() const;
    size_t h() const;
    size_t dim() const;
    size_t nPoints() const;
    float* points();
    float* result();
    tsDeBoorNet* data();
    
private:
    tsDeBoorNet deBoorNet;
};

class TsBSpline {
public:
    TsBSpline();
    TsBSpline(const TsBSpline& other);
    TsBSpline(const size_t deg, const size_t dim, const size_t nCtrlp,
              const tsBSplineType type);
    TsBSpline(const float* points, const size_t nPoints, const size_t dim);
    ~TsBSpline();

    TsBSpline& operator=(const TsBSpline& other);
    TsDeBoorNet operator()(const float u) const;
    
    size_t deg() const;
    size_t order() const;
    size_t dim() const;
    size_t nCtrlp() const;
    size_t nKnots() const;
    float* ctrlp();
    float* knots();
    tsBSpline* data();
    
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