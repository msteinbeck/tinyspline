%module tinysplinego

%ignore tinyspline::BSpline::operator();
%ignore tinyspline::BSpline::operator=;
%ignore tinyspline::DeBoorNet::operator=;
%ignore tinyspline::Domain::operator=;

%insert(cgo_comment_typedefs) %{
#cgo LDFLAGS: -L${SRCDIR} -ltinysplinego -Wl,-rpath,${SRCDIR}
%}

#ifdef TINYSPLINE_FLOAT_PRECISION
	%typemap(gotype) std::vector<tinyspline::real> * "[]float32"
#else
	%typemap(gotype) std::vector<tinyspline::real> * "[]float64"
#endif

// Map std::vector<tinyspline::real> to Go slice (do not free $1).
%typemap(out) std::vector<tinyspline::real> * {
	$result = {&$1->front(), $1->size(), $1->size()};
}

// Map Go slice to std::vector<tinyspline::real>.
%typemap(in) std::vector<tinyspline::real> * {
	tinyspline::real* data = (tinyspline::real *)$input.array;
	intgo size = $input.len;
	$1 = new std::vector<tinyspline::real>();
	$1->reserve(size);
	for (int i = 0; i < size; i++) {
		$1->push_back((tinyspline::real)data[i]);
	}
}
%typemap(freearg) std::vector<tinyspline::real> * {
	delete $1;
}

%include "tinyspline.i"
