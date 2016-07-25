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

    tsRational u() const;
    size_t k() const;
    size_t s() const;
    size_t h() const;
    size_t dim() const;
    size_t nPoints() const;
    std::vector<tsRational> points() const;
    std::vector<tsRational> result() const;
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
    BSpline(const std::vector<tsRational> points, const size_t dim);
    ~BSpline();

    /* Operators */
    BSpline& operator=(const BSpline& other);
    BSpline& operator=(BSpline&& other);
    DeBoorNet operator()(const tsRational u) const;

    /* Used by move semantics */
    void swap(BSpline& other);
    friend void swap(BSpline &left, BSpline &right) { left.swap(right); }

    /* Getter */
    size_t deg() const;
    size_t order() const;
    size_t dim() const;
    size_t nCtrlp() const;
    size_t nKnots() const;
    std::vector<tsRational> ctrlp() const;
    std::vector<tsRational> knots() const;
    tsBSpline* data();
    DeBoorNet evaluate(const tsRational u) const;

    /* Modifications */
    void setCtrlp(const std::vector<tsRational> ctrlp);
    void setKnots(const std::vector<tsRational> knots);

    /* Transformations */
    BSpline setupKnots(const tsBSplineType type,
                    const tsRational min, const tsRational max) const;
    BSpline insertKnot(const tsRational u, const size_t n) const;
    BSpline resize(const int n, const int back) const;
    BSpline split(const tsRational u) const;
    BSpline buckle(const tsRational b) const;
    BSpline toBeziers() const;
    BSpline derive() const;

private:
    tsBSpline bspline;
};

class Utils {
public:
    Utils() = delete;

    static bool fequals(const tsRational x, const tsRational y);
    static std::string enum_str(const tsError err);
    static tsError str_enum(const std::string str);
};

}
