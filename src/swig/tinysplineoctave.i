%module tinysplineoctave

%include "snake_case.swg"

// Map std::vector<tinyspline::real> to Octave vector.
%typemap(out) std::vector<tinyspline::real> * {
	const size_t size = $1->size();
	Matrix m(1, size);
	for (int i = 0; i < size; i++) {
		m(0, i) = (*$1)[i];
	}
	$result = m;
}
%typemap(newfree) std::vector<tinyspline::real> * {
	delete $1;
}

// Map Octave vector to std::vector<tinyspline::real>.
%typemap(in) std::vector<tinyspline::real> * (size_t size) %{
	size = $input.length();
	$1 = new std::vector<tinyspline::real>();
	$1->reserve(size);
	for (size_t i = 0; i < size; i++) {
		$1->push_back($input.array_value()(i));
	}
%}
%typemap(freearg) std::vector<tinyspline::real> * {
	delete $1;
}

%include "tinyspline.i"
