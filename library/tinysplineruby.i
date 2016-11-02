%module tinysplineruby

// std::vector<ts::rational> to ruby array
%typemap(out) std::vector<ts::rational> * {
  const size_t size = $1->size();
  $result = rb_ary_new2(size);
  for (int i = 0; i < size; i++) {
    rb_ary_store($result, i, DBL2NUM((*$1)[i]));
  }
}

// ruby array to std::vector<ts::rational>
%typemap(in) std::vector<ts::rational> * (size_t size) %{
  size = RARRAY_LEN($input);
  $1 = new std::vector<ts::rational>();
  $1->reserve(size);
  for (size_t i = 0; i < size; i++) {
    $1->push_back(NUM2DBL(rb_ary_entry($input, i)));
  }
%}

// cleanup memory allocated by typemaps
%typemap(freearg) std::vector<ts::rational> * {
  delete $1;
}

//********************************************************
//*                                                      *
//* BSpline (Ruby)                                       *
//*                                                      *
//********************************************************
%ignore ts::BSpline::operator=;

//********************************************************
//*                                                      *
//* DeBoorNet (Ruby)                                     *
//*                                                      *
//********************************************************
%ignore ts::DeBoorNet::operator=;

//********************************************************
//*                                                      *
//* SWIG base file                                       *
//*                                                      *
//********************************************************
%include "tinyspline.i"
