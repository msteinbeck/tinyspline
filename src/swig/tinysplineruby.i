%module tinysplineruby

%include "snake_case.swg"

%rename (inspect) tinyspline::BSpline::toString;
%rename (inspect) tinyspline::ChordLengths::toString;
%rename (inspect) tinyspline::DeBoorNet::toString;
%rename (inspect) tinyspline::Domain::toString;
%rename (inspect) tinyspline::Frame::toString;
%rename (inspect) tinyspline::FrameSeq::toString;
%rename (inspect) tinyspline::Morphism::toString;
%rename (inspect) tinyspline::Vec2::toString;
%rename (inspect) tinyspline::Vec3::toString;
%rename (inspect) tinyspline::Vec4::toString;

// Map std::vector<tinyspline::real> to Ruby array.
%typemap(out) std::vector<tinyspline::real> * {
	const size_t size = $1->size();
	$result = rb_ary_new2(size);
	for (int i = 0; i < size; i++) {
		rb_ary_store($result, i, DBL2NUM((*$1)[i]));
	}
}
%typemap(newfree) std::vector<tinyspline::real> * {
	delete $1;
}

// Map Ruby array to std::vector<tinyspline::real>.
%typemap(in) std::vector<tinyspline::real> * (size_t size) %{
	size = RARRAY_LEN($input);
	$1 = new std::vector<tinyspline::real>();
	$1->reserve(size);
	for (size_t i = 0; i < size; i++) {
		$1->push_back(NUM2DBL(rb_ary_entry($input, i)));
	}
%}
%typemap(freearg) std::vector<tinyspline::real> * {
	delete $1;
}

%include "tinyspline.i"
