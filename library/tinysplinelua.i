%module tinysplinelua

// std::vector<ts::rational> to lua table
%typemap(out) std::vector<ts::rational> * {
  const int size = $1->size();
  lua_createtable(L, size, 0);
  for (int i = 0; i < size; i++) {
    lua_pushnumber(L, (*$1)[i]);
    lua_rawseti(L, -2, i+1);
  }
  return 1;
}

// lua table to std::vector<ts::rational>
%typemap(in) std::vector<ts::rational> * (int idx) %{
  $1 = new std::vector<ts::rational>();
  idx = lua_gettop(L);
  lua_pushnil(L);
  while (lua_next(L, idx)) {
    $1->push_back(lua_tonumber(L, -1));
    lua_pop(L, 1);
  }
%}

// cleanup memory allocated by typemaps
%typemap(freearg) std::vector<ts::rational> * {
  delete $1;
}

//********************************************************
//*                                                      *
//* BSpline (Lua)                                        *
//*                                                      *
//********************************************************
%ignore ts::BSpline::operator=;

//********************************************************
//*                                                      *
//* DeBoorNet (Lua)                                      *
//*                                                      *
//********************************************************
%ignore ts::DeBoorNet::operator=;

//********************************************************
//*                                                      *
//* SWIG base file                                       *
//*                                                      *
//********************************************************
%include "tinyspline.i"
