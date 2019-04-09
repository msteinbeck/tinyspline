%module tinysplinelua

%ignore tinyspline::BSpline::operator=;
%ignore tinyspline::DeBoorNet::operator=;
%ignore tinyspline::Domain::operator=;

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

// Cleanup memory allocated by typemaps.
%typemap(freearg) std::vector<tinyspline::real> * {
	delete $1;
}

%include "tinyspline.i"
