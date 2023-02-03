%module tinysplinego

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

%rename (string) tinyspline::BSpline::toString;
%rename (string) tinyspline::ChordLengths::toString;
%rename (string) tinyspline::DeBoorNet::toString;
%rename (string) tinyspline::Domain::toString;
%rename (string) tinyspline::Frame::toString;
%rename (string) tinyspline::FrameSeq::toString;
%rename (string) tinyspline::Morphism::toString;
%rename (string) tinyspline::Vec2::toString;
%rename (string) tinyspline::Vec3::toString;
%rename (string) tinyspline::Vec4::toString;

%insert(cgo_comment_typedefs) %{
#cgo linux,386 LDFLAGS:    -L${SRCDIR} -L${SRCDIR}/linux-x86     -ltinysplinego -lm -lstdc++
#cgo linux,amd64 LDFLAGS:  -L${SRCDIR} -L${SRCDIR}/linux-x86_64  -ltinysplinego -lm -lstdc++
#cgo darwin,386 LDFLAGS:   -L${SRCDIR} -L${SRCDIR}/macosx-x86    -ltinysplinego -lm -lstdc++
#cgo darwin,amd64 LDFLAGS: -L${SRCDIR} -L${SRCDIR}/macosx-x86_64 -ltinysplinego -lm -lstdc++
#cgo darwin,arm64 LDFLAGS: -L${SRCDIR} -L${SRCDIR}/macosx-arm64  -ltinysplinego -lm -lstdc++
#cgo windows,386,!stdcxxshared LDFLAGS:   -L${SRCDIR} -L${SRCDIR}/windows-x86    -ltinysplinego -Wl,-Bstatic -lstdc++
#cgo windows,amd64,!stdcxxshared LDFLAGS: -L${SRCDIR} -L${SRCDIR}/windows-x86_64 -ltinysplinego -Wl,-Bstatic -lstdc++
#cgo windows,386,stdcxxshared LDFLAGS:    -L${SRCDIR} -L${SRCDIR}/windows-x86    -ltinysplinego -lstdc++-6
#cgo windows,amd64,stdcxxshared LDFLAGS:  -L${SRCDIR} -L${SRCDIR}/windows-x86_64 -ltinysplinego -lstdc++-6
%}

// Simplifies the API (no type cast required).
%typemap(gotype) size_t "int"

#ifdef TINYSPLINE_FLOAT_PRECISION
	%typemap(gotype) std::vector<tinyspline::real> * "[]float32"
#else
	%typemap(gotype) std::vector<tinyspline::real> * "[]float64"
#endif

// Map std::vector<tinyspline::real> to Go slice.
%typemap(out) std::vector<tinyspline::real> * {
	size_t size = $1->size() * sizeof(tinyspline::real);
	tinyspline::real *data = (tinyspline::real *)malloc(size);
	if (!data) throw std::bad_alloc();
	memcpy(data, &$1->front(), size);
	$result = {data, (intgo) $1->size(), (intgo) $1->size()};
}
%typemap(newfree) std::vector<tinyspline::real> * {
	delete $1;
}

// Map Go slice to std::vector<tinyspline::real>.
%typemap(in) std::vector<tinyspline::real> * {
	tinyspline::real *data = (tinyspline::real *)$input.array;
	$1 = new std::vector<tinyspline::real>(data, data + $input.len);
}
%typemap(freearg) std::vector<tinyspline::real> * {
	delete $1;
}

%include "tinyspline.i"
