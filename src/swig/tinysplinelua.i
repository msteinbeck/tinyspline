%module tinysplinelua

%include "snake_case.swg"

%rename (__tostring) tinyspline::BSpline::toString;
%rename (__tostring) tinyspline::ChordLengths::toString;
%rename (__tostring) tinyspline::DeBoorNet::toString;
%rename (__tostring) tinyspline::Domain::toString;
%rename (__tostring) tinyspline::Frame::toString;
%rename (__tostring) tinyspline::FrameSeq::toString;
%rename (__tostring) tinyspline::Morphism::toString;
%rename (__tostring) tinyspline::Vec2::toString;
%rename (__tostring) tinyspline::Vec3::toString;
%rename (__tostring) tinyspline::Vec4::toString;

// Map std::vector<tinyspline::real> to Lua table.
%typemap(out) std::vector<tinyspline::real> * {
	const int size = $1->size();
	lua_createtable(L, size, 0);
	for (int i = 0; i < size; i++) {
		lua_pushnumber(L, (*$1)[i]);
		lua_rawseti(L, -2, i+1);
	}
	return 1;
}
%typemap(newfree) std::vector<tinyspline::real> * {
	delete $1;
}

// Map Lua table to std::vector<tinyspline::real>.
%typemap(in) std::vector<tinyspline::real> * %{
	$1 = new std::vector<tinyspline::real>();
	lua_pushnil(L);
	while (lua_next(L, $input)) {
		$1->push_back(lua_tonumber(L, -1));
		lua_pop(L, 1);
	}
%}
%typemap(freearg) std::vector<tinyspline::real> * {
	delete $1;
}

%include "tinyspline.i"
