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
	explicit DeBoorNet(tsDeBoorNet &src);

	friend class BSpline;

#ifdef TINYSPLINE_EMSCRIPTEN
	#include <stdexcept>
public:
	DeBoorNet() : net(ts_deboornet_init()) {}
	void setKnot(real) { raise_exception(); }
	void setIndex(size_t) { raise_exception(); }
	void setMultiplicity(size_t) { raise_exception(); }
	void setNumInsertions(size_t) { raise_exception(); }
	void setDimension(size_t) { raise_exception(); }
	void setPoints(std::vector<real>) { raise_exception(); }
	void setResult(std::vector<real>) { raise_exception(); }
private:
	void inline raise_exception() {
		throw std::runtime_error("cannot write read-only property");
	}
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
	#include <stdexcept>
public:
	Domain() : _min(TS_DOMAIN_DEFAULT_MIN), _max(TS_DOMAIN_DEFAULT_MAX) {}
	void setMin(real) { raise_exception(); }
	void setMax(real) { raise_exception(); }
private:
	void inline raise_exception() {
		throw std::runtime_error("cannot write read-only property");
	}
#endif
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

#ifdef TINYSPLINE_EMSCRIPTEN
// https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html
// https://emscripten.org/docs/api_reference/bind.h.html
#include <emscripten/bind.h>
using namespace emscripten;
using namespace tinyspline;
EMSCRIPTEN_BINDINGS(tinyspline) {
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
	        .class_function("fromJson", &BSpline::fromJson)

	        .property("degree", &BSpline::degree)
	        .property("order", &BSpline::order)
	        .property("dimension", &BSpline::dimension)
	        .property("controlPoints", &BSpline::controlPoints,
			&BSpline::setControlPoints)
	        .property("knots", &BSpline::knots, &BSpline::setKnots)

	        /* Property by index */
	        .function("controlPointAt", &BSpline::controlPointAt)
	        .function("setControlPointAt", &BSpline::setControlPointAt)
	        .function("knotAt", &BSpline::knotAt)
	        .function("setKnotAt", &BSpline::setKnotAt)

	        /* Query */
	        .function("numControlPoints", &BSpline::numControlPoints)
	        .function("eval", &BSpline::eval)
	        .function("evalAll", &BSpline::evalAll)
	        .function("sample", &BSpline::sample)
	        .function("bisect", &BSpline::bisect)
	        .function("domain", &BSpline::domain)
	        .function("isClosed", &BSpline::isClosed)

		/* Serialization */
	        .function("toJson", &BSpline::toJson)

	        /* Transformations */
	        .function("insertKnot", &BSpline::insertKnot)
	        .function("split", &BSpline::split)
	        .function("tension", &BSpline::tension)
	        .function("toBeziers", &BSpline::toBeziers)
	        .function("derive", &BSpline::derive)

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
