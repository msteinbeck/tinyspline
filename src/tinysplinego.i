%module tinysplinego

%ignore tinyspline::BSpline::operator();
%ignore tinyspline::BSpline::operator=;
%ignore tinyspline::DeBoorNet::operator=;
%ignore tinyspline::Domain::operator=;

%insert(cgo_comment_typedefs) %{
#cgo LDFLAGS: -L${SRCDIR} -ltinysplinego -Wl,-rpath,${SRCDIR}
%}

%include "tinyspline.i"
