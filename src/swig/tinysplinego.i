%module tinysplinego

%ignore tinyspline::BSpline::operator();
%ignore tinyspline::BSpline::operator=;
%ignore tinyspline::DeBoorNet::operator=;
%ignore tinyspline::Domain::operator=;

%rename (string) tinyspline::BSpline::toString;
%rename (string) tinyspline::DeBoorNet::toString;
%rename (string) tinyspline::Domain::toString;

%insert(cgo_comment_typedefs) %{
#cgo LDFLAGS: -L${SRCDIR} -ltinysplinego -Wl,-rpath,${SRCDIR}
%}

// Simplify API so one can call a function without explicit type cast
// (from int to int64).
%typemap(gotype) size_t "int"

#ifdef TINYSPLINE_FLOAT_PRECISION
	%typemap(gotype) std::vector<tinyspline::real> * "[]float32"
#else
	%typemap(gotype) std::vector<tinyspline::real> * "[]float64"
#endif

// Map std::vector<tinyspline::real> to Go slice (do not free $1).
%typemap(out) std::vector<tinyspline::real> * {
	size_t size = $1->size() * sizeof(tinyspline::real);
	tinyspline::real *data = (tinyspline::real *)malloc(size);
	if (!data) throw std::bad_alloc();
	memcpy(data, &$1->front(), size);
	$result = {data, $1->size(), $1->size()};
}
%typemap(newfree) std::vector<tinyspline::real> * {
	delete $1;
}

// Map Go slice to std::vector<tinyspline::real>.
%typemap(in) std::vector<tinyspline::real> * {
	tinyspline::real *data = (tinyspline::real *)$input.array;
	$1 = new std::vector<tinyspline::real>(data, data + $input.len);
}
%typemap(freearg) std::vector<tinyspline::real> * {
	delete $1;
}

%include "tinyspline.i"
