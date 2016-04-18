#pragma once

#include "tinyspline.h"
#include <vector>
#include <string>

namespace ts {

class DeBoorNet {
public:
    DeBoorNet();
    DeBoorNet(const DeBoorNet& other);
    DeBoorNet(DeBoorNet&& other);
    ~DeBoorNet();

    DeBoorNet& operator=(const DeBoorNet& other);
    DeBoorNet& operator=(DeBoorNet&& other);

    void swap(DeBoorNet& other);
    friend void swap(DeBoorNet &left, DeBoorNet &right) { left.swap(right); }

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

class BSpline {
public:
    /* Constructors */
    BSpline();
    BSpline(const BSpline& other);
    BSpline(BSpline&& other);
    BSpline(const size_t deg, const size_t dim, const size_t nCtrlp,
            const tsBSplineType type);
    BSpline(const std::vector<float> points, const size_t dim);
    ~BSpline();

    /* Operators */
    BSpline& operator=(const BSpline& other);
    BSpline& operator=(BSpline&& other);
    DeBoorNet operator()(const float u) const;

    /* Used by move semantics */
    void swap(BSpline& other);
    friend void swap(BSpline &left, BSpline &right) { left.swap(right); }

    /* Getter */
    size_t deg() const;
    size_t order() const;
    size_t dim() const;
    size_t nCtrlp() const;
    size_t nKnots() const;
    std::vector<float> ctrlp() const;
    std::vector<float> knots() const;
    tsBSpline* data();
    DeBoorNet evaluate(const float u) const;

    /* Modifications */
    void setCtrlp(const std::vector<float> ctrlp);
    void setKnots(const std::vector<float> knots);

    /* Transformations */
    BSpline setupKnots(const tsBSplineType type,
                    const float min, const float max) const;
    BSpline insertKnot(const float u, const size_t n) const;
    BSpline resize(const int n, const int back) const;
    BSpline split(const float u) const;
    BSpline buckle(const float b) const;
    BSpline toBeziers() const;
    BSpline derive() const;

private:
    tsBSpline bspline;
};

class Utils {
public:
    Utils() = delete;

    static bool fequals(const float x, const float y);
    static std::string enum_str(const tsError err);
    static tsError str_enum(const std::string str);
};

}
