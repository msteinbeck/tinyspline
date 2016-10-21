%module tinysplinephp

// std::vector<ts::rational> to PHP native array
%typemap(out) std::vector<ts::rational> * {
  array_init($result);
  for (std::vector<ts::rational>::iterator it = $1->begin();
      it != $1->end(); it++) {
    zval *data;
    MAKE_STD_ZVAL(data);
    ZVAL_DOUBLE(data, *it);
    zend_hash_next_index_insert(HASH_OF($result), &data,
        sizeof(zval *), NULL);
  }
}

// PHP native array to std::vector<ts::rational>
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

// cleanup memory allocated by typemap
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
