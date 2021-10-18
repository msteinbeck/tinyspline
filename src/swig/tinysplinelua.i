%module tinysplinelua

%ignore tinyspline::BSpline::operator=;
%ignore tinyspline::DeBoorNet::operator=;
%ignore tinyspline::Domain::operator=;

%rename (__tostring) tinyspline::BSpline::toString;
%rename (__tostring) tinyspline::DeBoorNet::toString;
%rename (__tostring) tinyspline::Domain::toString;
%rename (__tostring) tinyspline::Frame::toString;
%rename (__tostring) tinyspline::FrameSeq::toString;
%rename (__tostring) tinyspline::Vec3::toString;

%rename("%(undercase)s", %$isfunction) "";
%rename("%(undercase)s", %$ismember, %$not %$isconstructor) "";

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
%typemap(in) std::vector<tinyspline::real> * (int idx) %{
	$1 = new std::vector<tinyspline::real>();
	idx = lua_gettop(L);
	lua_pushnil(L);
	while (lua_next(L, idx)) {
		$1->push_back(lua_tonumber(L, -1));
		lua_pop(L, 1);
	}
%}
%typemap(freearg) std::vector<tinyspline::real> * {
	delete $1;
}

%include "tinyspline.i"
