%module tinyspliner

%ignore tinyspline::BSpline::operator=;
%ignore tinyspline::DeBoorNet::operator=;
%ignore tinyspline::Domain::operator=;
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

// Cleanup memory allocated by typemaps.
%typemap(freearg) std::vector<tinyspline::real> * {
        delete $1;
}

// Swig's R module does not work very well with C typedefs. Thus, we need to
// create custom typemaps for tinyspline::real.

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
