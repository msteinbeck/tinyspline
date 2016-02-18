#pragma once

#include "tinyspline.h"
#include <vector>
#include <string>

namespace ts {

class DeBoorNet {
public:
    DeBoorNet();
    DeBoorNet(const DeBoorNet& other);
    ~DeBoorNet();

    DeBoorNet& operator=(const DeBoorNet& other);

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
    BSpline();
    BSpline(const BSpline& other);
    BSpline(const size_t deg, const size_t dim, const size_t nCtrlp,
            const tsBSplineType type);
    BSpline(const std::vector<float> points, const size_t dim);
    ~BSpline();

    BSpline& operator=(const BSpline& other);
    DeBoorNet operator()(const float u) const;

    size_t deg() const;
    size_t order() const;
    size_t dim() const;
    size_t nCtrlp() const;
    size_t nKnots() const;
    std::vector<float> ctrlp() const;
    std::vector<float> knots() const;
    tsBSpline* data();

    void setCtrlp(const std::vector<float> ctrlp);
    void setKnots(const std::vector<float> knots);

    void setupKnots(const tsBSplineType type,
                    const float min, const float max);
    void resize(const int n, const int back);
    void buckle(const float b);
    void toBeziers();
    size_t insertKnot(const float u, const size_t n);
    size_t split(const float u);
    DeBoorNet evaluate(const float u) const;

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
