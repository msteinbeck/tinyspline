%module tinysplineoctave

%ignore tinyspline::BSpline::operator=;
%ignore tinyspline::DeBoorNet::operator=;
%ignore tinyspline::Domain::operator=;

%rename("%(undercase)s", %$isfunction) "";
%rename("%(undercase)s", %$ismember, %$not %$isconstructor) "";

// Map std::vector<tinyspline::real> to Octave vector.
%typemap(out) std::vector<tinyspline::real> * {
	const size_t size = $1->size();
	Matrix m(1, size);
	for (int i = 0; i < size; i++) {
		m(0, i) = (*$1)[i];
	}
	$result = m;
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

// Cleanup memory allocated by typemaps.
%typemap(freearg) std::vector<tinyspline::real> * {
	delete $1;
}

%include "tinyspline.i"
