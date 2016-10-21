%module tinysplinephp

// std::vector<ts::rational> to php native array
%typemap(out) std::vector<ts::rational> * {
  const int size = $1->size();
  array_init_size($result, size);
  for (int i = 0; i < size; i++) {
    add_index_double($result, i, (*$1)[i]);
  }
}

// php native array to std::vector<ts::rational>
%typemap(in) std::vector<ts::rational> * %{
  $1 = new std::vector<ts::rational>();
  for (int idx = 0;; idx++) {
    zval **value;
    if (zend_hash_index_find(Z_ARRVAL_P(*$input), idx,
        (void**)&value) == SUCCESS) {
      convert_to_double_ex(value);
      $1->push_back(Z_DVAL_PP(value));
    } else {
      // no more indices in array
      break;
    }
  }
%}

// cleanup memory allocated by typemaps
%typemap(freearg) std::vector<ts::rational> * {
  delete $1;
}

//********************************************************
//*                                                      *
//* BSpline (PHP)                                        *
//*                                                      *
//********************************************************
%ignore ts::BSpline::operator=;
%ignore ts::BSpline::operator();

//********************************************************
//*                                                      *
//* DeBoorNet (PHP)                                      *
//*                                                      *
//********************************************************
%ignore ts::DeBoorNet::operator=;

//********************************************************
//*                                                      *
//* SWIG base file                                       *
//*                                                      *
//********************************************************
%include "tinyspline.i"
