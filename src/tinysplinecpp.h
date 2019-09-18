#pragma once

#include "tinyspline.h"
#include <vector>
#include <string>

#ifdef SWIG
#define std_real_vector_in std::vector<tinyspline::real> *
#define std_real_vector_out std::vector<tinyspline::real> *
#else
#define std_real_vector_in std::vector<tinyspline::real> &
#define std_real_vector_out std::vector<tinyspline::real>
#endif

namespace tinyspline {

typedef tsReal real;
class BSpline;

class DeBoorNet {
public:
	/* Constructors & Destructors */
	DeBoorNet(const DeBoorNet &other);
	~DeBoorNet();

	/* Operators */
	DeBoorNet & operator=(const DeBoorNet &other);

	/* Accessors */
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
	/* Constructors & Destructors */
	Domain(tsReal min, tsReal max);
	Domain(const Domain &other);

	/* Operators */
	Domain &operator=(const Domain &other);

	/* Getter */
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

	/* Accessors */
	size_t degree() const;
	size_t order() const;
	size_t dimension() const;
	std::vector<real> controlPoints() const;
	std_real_vector_out controlPointAt(size_t index) const;
	std::vector<real> knots() const;
	tsBSpline * data();

	/* Query */
	size_t numDistinctKnots() const;
	DeBoorNet eval(real u) const;
	std_real_vector_out evalAll(const std_real_vector_in us) const;
	std_real_vector_out sample(size_t num = 0) const;
	DeBoorNet bisect(real value, real epsilon = 0.01,
		bool persnickety = false, size_t index = 0,
		bool ascending = true, size_t maxIter = 30) const;
	Domain domain() const;
	bool isClosed(real epsilon = 0.00001) const;

	/* Serialization */
	std::string toJSON();
	void fromJSON(std::string json);
	void save(std::string path);
	void load(std::string path);

	/* Modifications */
	void setControlPoints(const std::vector<real> &ctrlp);
	void setControlPointAt(size_t index, const std_real_vector_in ctrlp);
	void setKnots(const std::vector<real> &knots);

	/* Transformations */
	BSpline insertKnot(real u, size_t n) const;
	BSpline split(real u) const;
	BSpline tension(real tension) const;
	BSpline toBeziers() const;
	BSpline derive(size_t n = 1) const;

private:
	tsBSpline spline;
};

class Utils {
public:
	static BSpline interpolateCubic(
		const std::vector<real> *points, size_t dim);
	static bool knotsEqual(real x, real y);

private:
	Utils() {}
};

}
