#pragma once

#include "tinyspline.h"
#include <vector>
#include <string>

namespace tinyspline {

typedef tsReal real;

class DeBoorNet {
public:
	/* Constructors & Destructors */
	DeBoorNet();
	DeBoorNet(const DeBoorNet &other);
	~DeBoorNet();

	/* Operators */
	DeBoorNet& operator=(const DeBoorNet &other);

	/* Getter */
	real u() const;
	size_t k() const;
	size_t s() const;
	size_t h() const;
	size_t dim() const;
	size_t nPoints() const;
	std::vector<real> points() const;
	std::vector<real> result() const;
	tsDeBoorNet * data();

	/* C++11 features */
#ifndef TINYSPLINE_DISABLE_CXX11_FEATURES
	DeBoorNet(DeBoorNet &&other);
	DeBoorNet & operator=(DeBoorNet &&other);
	void swap(DeBoorNet &other);
	friend void swap(DeBoorNet &left, DeBoorNet &right)
	{
		left.swap(right);
	}
#endif

private:
	tsDeBoorNet deBoorNet;
};

class BSpline {
public:
	/* Constructors & Destructors */
	BSpline();
	BSpline(const BSpline &other);
	BSpline(const size_t deg, const size_t dim, const size_t nCtrlp,
		const tsBSplineType type);
	~BSpline();

	/* Operators */
	BSpline & operator=(const BSpline &other);
	DeBoorNet operator()(const real u) const;

	/* Getter */
	size_t deg() const;
	size_t order() const;
	size_t dim() const;
	size_t nCtrlp() const;
	size_t nKnots() const;
	std::vector<real> ctrlp() const;
	std::vector<real> knots() const;
	tsBSpline * data();
	DeBoorNet evaluate(const real u) const;

	/* Modifications */
	void setCtrlp(const std::vector<real> &ctrlp);
	void setKnots(const std::vector<real> &knots);

	/* Transformations */
	BSpline fillKnots(const tsBSplineType type, const real min,
		const real max) const;
	BSpline insertKnot(const real u, const size_t n) const;
	BSpline resize(const int n, const int back) const;
	BSpline split(const real u) const;
	BSpline buckle(const real b) const;
	BSpline toBeziers() const;
	BSpline derive() const;

	/* C++11 features */
#ifndef TINYSPLINE_DISABLE_CXX11_FEATURES
	BSpline(BSpline &&other);
	BSpline& operator=(BSpline &&other);
	void swap(BSpline &other);
	friend void swap(BSpline &left, BSpline &right)
	{
		left.swap(right);
	}
#endif

private:
	tsBSpline bspline;
};

class Utils {
public:
	static BSpline interpolateCubic(const std::vector<real> *points,
		const size_t dim);
	static bool fequals(const real x, const real y);
	static std::string enum_str(const tsError err);
	static tsError str_enum(const std::string str);

private:
	Utils() {}
};

}
