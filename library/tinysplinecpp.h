#pragma once

#include "tinyspline.h"
#include <vector>

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
    std::vector<float> points() const;
    std::vector<float> result() const;
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
    TsBSpline(const std::vector<float> points, const size_t dim);
    ~TsBSpline();

    TsBSpline& operator=(const TsBSpline& other);
    TsDeBoorNet operator()(const float u) const;
    
    size_t deg() const;
    size_t order() const;
    size_t dim() const;
    size_t nCtrlp() const;
    size_t nKnots() const;
    std::vector<float> ctrlp() const;
    std::vector<float> knots() const;
    tsBSpline* data();
    
    void setDeg(const size_t deg);
    void setOrder(const size_t order);
    void setCtrlp(const std::vector<float> ctrlp);
    void setKnots(const std::vector<float> knots);
    
    void setupKnots(const tsBSplineType type,
                    const float min, const float max);
    void resize(const int n, const int back);
    void buckle(const float b);
    void toBeziers();
    size_t insertKnot(const float u, const size_t n);
    size_t split(const float u);
    TsDeBoorNet evaluate(const float u) const;
    
private:
    tsBSpline bspline;
};