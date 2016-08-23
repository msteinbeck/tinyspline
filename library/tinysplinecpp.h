#pragma once

#include "tinyspline.h"
#include <vector>
#include <string>

namespace ts {

typedef tsRational rational;

class DeBoorNet {
public:
    /* Constructors & Destructors */
    DeBoorNet();
    DeBoorNet(const DeBoorNet& other);
    ~DeBoorNet();

    /* Operators */
    DeBoorNet& operator=(const DeBoorNet& other);

    /* Getter */
    rational u() const;
    size_t k() const;
    size_t s() const;
    size_t h() const;
    size_t dim() const;
    size_t nPoints() const;
    std::vector<rational> points() const;
    std::vector<rational> result() const;
    tsDeBoorNet* data();

    /* C++11 features */
#ifndef TINYSPLINE_DISABLE_CXX11_FEATURES
    DeBoorNet(DeBoorNet&& other);
    DeBoorNet& operator=(DeBoorNet&& other);
    void swap(DeBoorNet& other);
    friend void swap(DeBoorNet &left, DeBoorNet &right) { left.swap(right); }
#endif

private:
    tsDeBoorNet deBoorNet;
};

class BSpline {
public:
    /* Constructors & Destructors */
    BSpline();
    BSpline(const BSpline& other);
    BSpline(const size_t deg, const size_t dim, const size_t nCtrlp,
            const tsBSplineType type);
    BSpline(const std::vector<rational> points, const size_t dim);
    ~BSpline();

    /* Operators */
    BSpline& operator=(const BSpline& other);
    DeBoorNet operator()(const rational u) const;

    /* Getter */
    size_t deg() const;
    size_t order() const;
    size_t dim() const;
    size_t nCtrlp() const;
    size_t nKnots() const;
    std::vector<rational> ctrlp() const;
    std::vector<rational> knots() const;
    tsBSpline* data();
    DeBoorNet evaluate(const rational u) const;

    /* Modifications */
    void setCtrlp(const std::vector<rational> ctrlp);
    void setKnots(const std::vector<rational> knots);

    /* Transformations */
    BSpline setupKnots(const tsBSplineType type,
                    const rational min, const rational max) const;
    BSpline insertKnot(const rational u, const size_t n) const;
    BSpline resize(const int n, const int back) const;
    BSpline split(const rational u) const;
    BSpline buckle(const rational b) const;
    BSpline toBeziers() const;
    BSpline derive() const;

    /* C++11 features */
#ifndef TINYSPLINE_DISABLE_CXX11_FEATURES
    BSpline(BSpline&& other);
    BSpline& operator=(BSpline&& other);
    void swap(BSpline& other);
    friend void swap(BSpline &left, BSpline &right) { left.swap(right); }
#endif

private:
    tsBSpline bspline;
};

class Utils {
public:
    static bool fequals(const rational x, const rational y);
    static std::string enum_str(const tsError err);
    static tsError str_enum(const std::string str);

private:
    Utils() {}
};

}
