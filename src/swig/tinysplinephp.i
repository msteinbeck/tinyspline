%module tinysplinephp

%ignore tinyspline::BSpline::operator();
%ignore tinyspline::Morphism::operator();
%ignore tinyspline::Vec2::operator+;
%ignore tinyspline::Vec2::operator-;
%ignore tinyspline::Vec2::operator*;
%ignore tinyspline::Vec3::operator+;
%ignore tinyspline::Vec3::operator-;
%ignore tinyspline::Vec3::operator*;
%ignore tinyspline::Vec4::operator+;
%ignore tinyspline::Vec4::operator-;
%ignore tinyspline::Vec4::operator*;

%rename (__toString) tinyspline::BSpline::toString;
%rename (__toString) tinyspline::ChordLengths::toString;
%rename (__toString) tinyspline::DeBoorNet::toString;
%rename (__toString) tinyspline::Domain::toString;
%rename (__toString) tinyspline::Frame::toString;
%rename (__toString) tinyspline::FrameSeq::toString;
%rename (__toString) tinyspline::Morphism::toString;
%rename (__toString) tinyspline::Vec2::toString;
%rename (__toString) tinyspline::Vec3::toString;
%rename (__toString) tinyspline::Vec4::toString;

// Map std::vector<tinyspline::real> to PHP array.
%typemap(out) std::vector<tinyspline::real> * {
	const int size = $1->size();
	array_init_size($result, size);
	for (int i = 0; i < size; i++) {
		add_index_double($result, i, (*$1)[i]);
	}
}
%typemap(newfree) std::vector<tinyspline::real> * {
	delete $1;
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
%typemap(freearg) std::vector<tinyspline::real> * {
	delete $1;
}

%include "tinyspline.i"
