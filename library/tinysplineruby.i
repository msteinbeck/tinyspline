%module tinysplineruby

// Map std::vector<tinyspline::rational> to Ruby array.
%typemap(out) std::vector<tinyspline::rational> * {
	const size_t size = $1->size();
	$result = rb_ary_new2(size);
	for (int i = 0; i < size; i++) {
		rb_ary_store($result, i, DBL2NUM((*$1)[i]));
	}
}

// Map Ruby array to std::vector<tinyspline::rational>.
%typemap(in) std::vector<tinyspline::rational> * (size_t size) %{
	size = RARRAY_LEN($input);
	$1 = new std::vector<tinyspline::rational>();
	$1->reserve(size);
	for (size_t i = 0; i < size; i++) {
		$1->push_back(NUM2DBL(rb_ary_entry($input, i)));
	}
%}

// Cleanup memory allocated by typemaps.
%typemap(freearg) std::vector<tinyspline::rational> * {
	delete $1;
}

//********************************************************
//*                                                      *
//* BSpline (Ruby)                                       *
//*                                                      *
//********************************************************
%ignore tinyspline::BSpline::operator=;

//********************************************************
//*                                                      *
//* DeBoorNet (Ruby)                                     *
//*                                                      *
//********************************************************
%ignore tinyspline::DeBoorNet::operator=;

//********************************************************
//*                                                      *
//* SWIG base file                                       *
//*                                                      *
//********************************************************
%include "tinyspline.i"
