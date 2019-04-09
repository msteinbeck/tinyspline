%module tinysplinephp

%ignore tinyspline::BSpline::operator=;
%ignore tinyspline::BSpline::operator();
%ignore tinyspline::DeBoorNet::operator=;
%ignore tinyspline::Domain::operator=;

// Map std::vector<tinyspline::real> to PHP array.
%typemap(out) std::vector<tinyspline::real> * {
	const int size = $1->size();
	array_init_size($result, size);
	for (int i = 0; i < size; i++) {
		add_index_double($result, i, (*$1)[i]);
	}
}

// Map PHP array to std::vector<tinyspline::real>.
%typemap(in) std::vector<tinyspline::real> * {
	$1 = new std::vector<tinyspline::real>();
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
%typemap(freearg) std::vector<tinyspline::real> * {
	delete $1;
}

%include "tinyspline.i"
