%module tinysplinephp

// Map std::vector<tinyspline::rational> to PHP array.
%typemap(out) std::vector<tinyspline::rational> * {
	const int size = $1->size();
	array_init_size($result, size);
	for (int i = 0; i < size; i++) {
		add_index_double($result, i, (*$1)[i]);
	}
}

// Map PHP array to std::vector<tinyspline::rational>.
%typemap(in) std::vector<tinyspline::rational> * {
	$1 = new std::vector<tinyspline::rational>();
	for (int idx = 0;; idx++) {
#ifdef SWIGPHP5
		zval **value;
		if (zend_hash_index_find(Z_ARRVAL_P(*$input), idx, (void**)&value) == SUCCESS) {
			convert_to_double_ex(value);
			$1->push_back(Z_DVAL_PP(value));
#else
		zval *value;
		value = zend_hash_index_find(Z_ARRVAL_P(&$input), idx);
		if (value) {
			convert_to_double_ex(value);
			$1->push_back(Z_DVAL_P(value));
#endif
		} else {
			// no more indices in array
			break;
		}
	}
}

// Cleanup memory allocated by typemaps.
%typemap(freearg) std::vector<tinyspline::rational> * {
	delete $1;
}

//********************************************************
//*                                                      *
//* BSpline (PHP)                                        *
//*                                                      *
//********************************************************
%ignore tinyspline::BSpline::operator=;
%ignore tinyspline::BSpline::operator();

//********************************************************
//*                                                      *
//* DeBoorNet (PHP)                                      *
//*                                                      *
//********************************************************
%ignore tinyspline::DeBoorNet::operator=;

//********************************************************
//*                                                      *
//* SWIG base file                                       *
//*                                                      *
//********************************************************
%include "tinyspline.i"
