%module tinyspliner

%ignore ts_bspline_insert_knot;
%ignore ts_deboornet_result;

// Map std::vector<tinyspline::real> to R vector.
%typemap(scoerceout) std::vector<tinyspline::real> * %{ %}
%typemap(out) std::vector<tinyspline::real> * {
	$result = Rcpp::wrap(*$1);
}

// Map R vector to std::vector<tinyspline::real>.
%typemap(in) std::vector<tinyspline::real> * {
	const std::vector<tinyspline::real> vec =
		Rcpp::as<std::vector<tinyspline::real> >($input);
	$1 = new std::vector<tinyspline::real>(vec);
}
%typemap(freearg) std::vector<tinyspline::real> * {
        delete $1;
}

// Swig's R module doesn't handle C typedefs well. Thus, we need to create
// custom typemaps for tinyspline::real.

// Add Rcpp to the generated C++ file.
%{
        #include "Rcpp.h"
%}

// Map tinyspline::real to R double
%typemap(scoerceout) tinyspline::real %{ %}
%typemap(out) tinyspline::real {
	$result = Rcpp::wrap($1);
}

// Map R double to tinyspline::real
%typemap(in) tinyspline::real {
	$1 = Rcpp::as<tinyspline::real>($input);
}

%include "tinyspline.i"
