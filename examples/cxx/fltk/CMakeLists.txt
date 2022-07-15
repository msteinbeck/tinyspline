if(MINGW)
  set(CMAKE_CXX_FLAGS "-static -static-libgcc -static-libstdc++")
endif()

find_package(FLTK)
if(FLTK_FOUND AND OPENGL_FOUND)
  include_directories(${FLTK_INCLUDE_DIR})
  include_directories(${CMAKE_CURRENT_SOURCE_DIR})

  fltk_wrap_ui(demo-ui demo.fl)
  add_library(demo-ui ${demo-ui_FLTK_UI_SRCS})
  if(${TINYSPLINE_PLATFORM_IS_WINDOWS})
    target_compile_definitions(demo-ui PUBLIC WIN32)
  endif()
  target_link_libraries(
    demo-ui PUBLIC ${FLTK_LIBRARIES} ${OPENGL_LIBRARIES} tinysplinecxx
  )

  add_executable(demo main.cxx eval.h sample.h frames.h)
  add_dependencies(demo demo-ui)
  target_link_libraries(demo PUBLIC demo-ui)
endif()
