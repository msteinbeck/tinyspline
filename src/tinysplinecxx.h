#pragma once

#include "tinyspline.h"
#include <vector>
#include <string>

#ifdef TINYSPLINECXX_SHARED
#ifdef TINYSPLINECXX_EXPORT
#define TINYSPLINECXX_API TINYSPLINE_SHARED_EXPORT
#else
#define TINYSPLINECXX_API TINYSPLINE_SHARED_IMPORT
#endif
#else
#define TINYSPLINECXX_API
#endif

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

class TINYSPLINECXX_API DeBoorNet {
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

	/* Debug */
	std::string toString() const;

private:
	tsDeBoorNet net;

	/* Constructors & Destructors */
	DeBoorNet();

	friend class BSpline;
};

class TINYSPLINECXX_API Domain {
public:
	/* Constructors & Destructors */
	Domain(tsReal min, tsReal max);
	Domain(const Domain &other);

	/* Operators */
	Domain &operator=(const Domain &other);

	/* Accessors */
	real min() const;
	real max() const;

	/* Debug */
	std::string toString() const;

private:
    real _min;
    real _max;
};

class TINYSPLINECXX_API BSpline {
public:
	typedef tsBSplineType type;

	/* Constructors & Destructors */
	BSpline();
	BSpline(const BSpline &other);
	explicit BSpline(size_t numControlPoints, size_t dimension = 2,
		size_t degree = 3,
		tinyspline::BSpline::type type = TS_CLAMPED);
	~BSpline();

	/* Create from static method */
	static BSpline interpolateCubicNatural(const std_real_vector_in points,
		size_t dimension);
	static BSpline interpolateCatmullRom(const std_real_vector_in points,
		size_t dimension, tsReal alpha = (tsReal) 0.5f,
		std::vector<tinyspline::real> *first = NULL,
		std::vector<tinyspline::real> *last = NULL,
		tsReal epsilon = TS_CONTROL_POINT_EPSILON);
	static BSpline fromJson(std::string json);
	static BSpline load(std::string path);

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
	real knotAt(size_t index) const;
	tsBSpline * data();

	/* Query */
	size_t numControlPoints() const;
	DeBoorNet eval(real u) const;
	std_real_vector_out evalAll(const std_real_vector_in us) const;
	std_real_vector_out sample(size_t num = 0) const;
	DeBoorNet bisect(real value, real epsilon = TS_CONTROL_POINT_EPSILON,
		bool persnickety = false, size_t index = 0,
		bool ascending = true, size_t maxIter = 30) const;
	Domain domain() const;
	bool isClosed(real epsilon = TS_CONTROL_POINT_EPSILON) const;

	/* Serialization */
	std::string toJson() const;
	void save(std::string path) const;

	/* Modifications */
	void setControlPoints(const std::vector<real> &ctrlp);
	void setControlPointAt(size_t index, const std_real_vector_in ctrlp);
	void setKnots(const std::vector<real> &knots);
	void setKnotAt(size_t index, real knot);

	/* Transformations */
	BSpline insertKnot(real u, size_t n) const;
	BSpline split(real u) const;
	BSpline tension(real tension) const;
	BSpline toBeziers() const;
	BSpline derive(size_t n = 1,
		real epsilon = TS_CONTROL_POINT_EPSILON) const;

	/* Debug */
	std::string toString() const;

private:
	tsBSpline spline;
};

class TINYSPLINECXX_API Utils {
public:
	static bool knotsEqual(real x, real y);

private:
	Utils() {}
};

}
