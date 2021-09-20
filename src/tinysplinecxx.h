#pragma once

#include "tinyspline.h"
#include <vector>
#include <string>

#ifndef TINYSPLINECXX_API
#define TINYSPLINECXX_API TINYSPLINE_API
#endif

#ifdef SWIG
#define std_real_vector_in std::vector<tinyspline::real> *
#define std_real_vector_out std::vector<tinyspline::real> *
#else
#define std_real_vector_in std::vector<tinyspline::real> &
#define std_real_vector_out std::vector<tinyspline::real>
#endif

#ifdef TINYSPLINE_EMSCRIPTEN
#include <stdexcept>
void inline cannotWrite() {
	throw std::runtime_error("cannot write read-only property");
}

std::string exceptionMessage(int ptr) {
	return std::string(reinterpret_cast<std::exception *>(ptr)->what());
}
#endif

namespace tinyspline {

typedef tsReal real;
class BSpline;
class Morphism;

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
	explicit DeBoorNet(tsDeBoorNet &data);

	friend class BSpline;

#ifdef TINYSPLINE_EMSCRIPTEN
public:
	DeBoorNet() : net(ts_deboornet_init()) {}
	void setKnot(real) { cannotWrite(); }
	void setIndex(size_t) { cannotWrite(); }
	void setMultiplicity(size_t) { cannotWrite(); }
	void setNumInsertions(size_t) { cannotWrite(); }
	void setDimension(size_t) { cannotWrite(); }
	void setPoints(std::vector<real>) { cannotWrite(); }
	void setResult(std::vector<real>) { cannotWrite(); }
#endif
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

#ifdef TINYSPLINE_EMSCRIPTEN
public:
	Domain() : _min(TS_DOMAIN_DEFAULT_MIN), _max(TS_DOMAIN_DEFAULT_MAX) {}
	void setMin(real) { cannotWrite(); }
	void setMax(real) { cannotWrite(); }
#endif
};

class TINYSPLINECXX_API Vector3 {
public:
	Vector3();
	Vector3(real x, real y, real z);
	Vector3(const Vector3 &other);

	Vector3 &operator=(const Vector3 &other);
	Vector3 operator+(const Vector3 &other);
	Vector3 operator-(const Vector3 &other);
	Vector3 operator*(real scalar);

	real x() const;
	void setX(real val);

	real y() const;
	void setY(real val);

	real z() const;
	void setZ(real val);

	Vector3 add(const Vector3 &other) const;
	Vector3 subtract(const Vector3 &other) const;
	Vector3 multiply(real scalar) const;
	Vector3 cross(const Vector3 &other) const;
	Vector3 norm() const;
	tsReal magnitude() const;
	tsReal dot(const Vector3 &other) const;

private:
	real vals[3];
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
	static BSpline parseJson(std::string json);
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
	BSpline elevateDegree(size_t amount,
		real epsilon = TS_CONTROL_POINT_EPSILON) const;
	BSpline alignWith(const BSpline &other, BSpline &otherAligned,
		real epsilon = TS_CONTROL_POINT_EPSILON) const;
	Morphism morphTo(const BSpline &other,
		real epsilon = TS_CONTROL_POINT_EPSILON) const;

	/* Debug */
	std::string toString() const;

private:
	tsBSpline spline;

	/* Constructors & Destructors */
	explicit BSpline(tsBSpline &data);

	/* Needs to access ::spline. */
	friend class Morphism;

#ifdef TINYSPLINE_EMSCRIPTEN
public:
	std_real_vector_out sample0() const { return sample(); }
	std_real_vector_out sample1(size_t num) const { return sample(num); }
	BSpline derive0() const { return derive(); }
	BSpline derive1(size_t n) const { return derive(n); }
	BSpline derive2(size_t n, real eps) const { return derive(n, eps); }
#endif
};

class TINYSPLINECXX_API Morphism {
public:
	/* Constructors & Destructors */
	Morphism(const BSpline &start, const BSpline &end,
		real epsilon = TS_CONTROL_POINT_EPSILON);

	BSpline eval(real t);
	BSpline operator()(real t);
private:
	BSpline start, end;
	real epsilon;
	BSpline startAligned, endAligned;
	BSpline buffer;
};

class TINYSPLINECXX_API Utils {
public:
	static bool knotsEqual(real x, real y);

private:
	Utils() {}
};

}

#ifdef TINYSPLINE_EMSCRIPTEN
// https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html
// https://emscripten.org/docs/api_reference/bind.h.html
#include <emscripten/bind.h>
using namespace emscripten;
using namespace tinyspline;
EMSCRIPTEN_BINDINGS(tinyspline) {
	function("exceptionMessage", &exceptionMessage);

	value_object<DeBoorNet>("DeBoorNet")
	        .field("knot", &DeBoorNet::knot, &DeBoorNet::setKnot)
	        .field("index", &DeBoorNet::index, &DeBoorNet::setIndex)
	        .field("multiplicity", &DeBoorNet::multiplicity,
	        	&DeBoorNet::setMultiplicity)
	        .field("numInsertions", &DeBoorNet::numInsertions,
	        	&DeBoorNet::setNumInsertions)
	        .field("dimension", &DeBoorNet::dimension,
	        	&DeBoorNet::setDimension)
	        .field("points", &DeBoorNet::points, &DeBoorNet::setPoints)
	        .field("result", &DeBoorNet::result, &DeBoorNet::setResult)
	;

	value_object<Domain>("Domain")
	        .field("min", &Domain::min, &Domain::setMin)
	        .field("max", &Domain::max, &Domain::setMax)
	;

	class_<BSpline>("BSpline")
	        .constructor<>()
	        //.constructor<BSpline>()
	        .constructor<size_t>()
	        .constructor<size_t, size_t>()
	        .constructor<size_t, size_t, size_t>()
	        .constructor<size_t, size_t, size_t, BSpline::type>()

	        .class_function("interpolateCubicNatural",
			&BSpline::interpolateCubicNatural)
	        .class_function("interpolateCatmullRom",
			&BSpline::interpolateCatmullRom,
			allow_raw_pointers())
	        .class_function("parseJson", &BSpline::parseJson)

	        .property("degree", &BSpline::degree)
	        .property("order", &BSpline::order)
	        .property("dimension", &BSpline::dimension)
	        .property("controlPoints", &BSpline::controlPoints,
			&BSpline::setControlPoints)
	        .property("knots", &BSpline::knots, &BSpline::setKnots)
	        .property("domain", &BSpline::domain)

	        /* Property by index */
	        .function("controlPointAt", &BSpline::controlPointAt)
	        .function("setControlPointAt", &BSpline::setControlPointAt)
	        .function("knotAt", &BSpline::knotAt)
	        .function("setKnotAt", &BSpline::setKnotAt)

	        /* Query */
	        .function("numControlPoints", &BSpline::numControlPoints)
	        .function("eval", &BSpline::eval)
	        .function("evalAll", &BSpline::evalAll)
	        .function("sample",
			select_overload<std_real_vector_out() const>
			(&BSpline::sample0))
	        .function("sample",
			select_overload<std_real_vector_out(size_t) const>
			(&BSpline::sample1))
	        .function("sample", &BSpline::sample)
	        .function("bisect", &BSpline::bisect)
	        .function("isClosed", &BSpline::isClosed)

		/* Serialization */
	        .function("toJson", &BSpline::toJson)

	        /* Transformations */
	        .function("insertKnot", &BSpline::insertKnot)
	        .function("split", &BSpline::split)
	        .function("tension", &BSpline::tension)
	        .function("toBeziers", &BSpline::toBeziers)
	        .function("derive",
			select_overload<BSpline() const>
			(&BSpline::derive0))
	        .function("derive",
			select_overload<BSpline(size_t) const>
			(&BSpline::derive1))
	        .function("derive",
			select_overload<BSpline(size_t, real) const>
			(&BSpline::derive2))

	        /* Debug */
	        .function("toString", &BSpline::toString)
	;

	enum_<BSpline::type>("BSplineType")
	        .value("OPENED", BSpline::type::TS_OPENED)
	        .value("CLAMPED", BSpline::type::TS_CLAMPED)
	        .value("BEZIERS", BSpline::type::TS_BEZIERS)
	;
}

// Map: std::vector <--> JS array
// https://github.com/emscripten-core/emscripten/issues/11070#issuecomment-717675128
namespace emscripten {
namespace internal {
	template <typename T, typename Allocator>
	struct BindingType<std::vector<T, Allocator>> {
		using ValBinding = BindingType<val>;
		using WireType = ValBinding::WireType;

		static WireType toWireType(
				const std::vector<T, Allocator> &vec) {
			return ValBinding::toWireType(val::array(vec));
		}

		static std::vector<T, Allocator> fromWireType(WireType value) {
			return vecFromJSArray<T>(
				ValBinding::fromWireType(value));
		}
	};

	template <typename T>
	struct TypeID<T,
		typename std::enable_if_t<std::is_same<
		        typename Canonicalized<T>::type,
		std::vector<typename Canonicalized<T>::type::value_type,
		typename Canonicalized<T>::type::allocator_type>>::value>> {

		static constexpr TYPEID get() {
			return TypeID<val>::get();
		}
	};
}  // namespace internal
}  // namespace emscripten
#endif
