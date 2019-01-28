#pragma once

#include "tinyspline.h"
#include <vector>
#include <string>

namespace tinyspline {

typedef tsReal real;
class BSpline;

class DeBoorNet {
public:
	/* Constructors & Destructors */
	DeBoorNet(const DeBoorNet &other);
	~DeBoorNet();

	/* Operators */
	DeBoorNet& operator=(const DeBoorNet &other);

	/* Getter */
	real knot() const;
	size_t index() const;
	size_t multiplicity() const;
	size_t numInsertions() const;
	size_t dimension() const;
	std::vector<real> points() const;
	std::vector<real> result() const;
	tsDeBoorNet * data();

private:
	tsDeBoorNet net;

	/* Constructors & Destructors */
	DeBoorNet();

	friend class BSpline;
};

class Domain {
public:
    Domain(tsReal min, tsReal max);
    Domain(const Domain &other);

    real min() const;
    real max() const;
private:
    real _min;
    real _max;
};

class BSpline {
public:
	typedef tsBSplineType type;

	/* Constructors & Destructors */
	BSpline();
	BSpline(const BSpline &other);
	explicit BSpline(size_t nCtrlp, size_t dim = 2, size_t deg = 3,
		tinyspline::BSpline::type type = TS_CLAMPED);
	~BSpline();

	/* Operators */
	BSpline & operator=(const BSpline &other);
	DeBoorNet operator()(real u) const;

	/* Getter */
	size_t degree() const;
	size_t order() const;
	size_t dimension() const;
	std::vector<real> controlPoints() const;
#ifdef SWIG
	std::vector<real> *
#else
	std::vector<real>
#endif
	controlPointAt(size_t index) const;
	std::vector<real> knots() const;
	tsBSpline * data();

	/* Query */
	DeBoorNet eval(real u) const;
	Domain domain() const;
	bool isClosed(real epsilon = 0.00001) const;

	/* Serialization */
	std::string toJSON();
	void fromJSON(std::string json);
	void save(std::string path);
	void load(std::string path);

	/* Modifications */
	void setControlPoints(const std::vector<real> &ctrlp);
	void setControlPointAt(size_t index,
#ifdef SWIG
	const std::vector<real> *ctrlp);
#else
	const std::vector<real> &ctrlp);
#endif
	void setKnots(const std::vector<real> &knots);

	/* Transformations */
	BSpline insertKnot(real u, size_t n) const;
	BSpline split(real u) const;
	BSpline buckle(real b) const;
	BSpline toBeziers() const;
	BSpline derive(size_t n = 1) const;

private:
	tsBSpline spline;
};

class Utils {
public:
	static BSpline interpolateCubic(
		const std::vector<real> *points, size_t dim);
	static bool fequals(real x, real y);

private:
	Utils() {}
};

}
