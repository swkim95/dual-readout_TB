cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(dual-readout_TB)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

find_package(ROOT REQUIRED)
find_package(PythonLibs REQUIRED)
find_package(Boost COMPONENTS python REQUIRED)

include_directories(
  ${PYTHON_INCLUDE_DIRS}
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

file(GLOB sources
  ${PROJECT_SOURCE_DIR}/src/*.cc
)

file(GLOB headers
  ${PROJECT_SOURCE_DIR}/include/*.h
)

include(${ROOT_USE_FILE})
ROOT_GENERATE_DICTIONARY(G__drcTB ${headers} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/include/LinkDef.hh MODULE drcTB)

add_library(drcTB SHARED ${sources} G__drcTB.cxx)
add_library(pydrcTB SHARED ${PROJECT_SOURCE_DIR}/src/pydrcTB.cpp)

target_include_directories(drcTB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${ROOT_INCLUDE_DIRS})

target_link_libraries(
  drcTB
  ${ROOT_LIBRARIES}
  Boost::python
  ${PYTHON_LIBRARIES}
)

target_link_libraries(
  pydrcTB
  drcTB
  Boost::python
  ${PYTHON_LIBRARIES}
)

set_target_properties(pydrcTB PROPERTIES SUFFIX .so)
set_target_properties(pydrcTB PROPERTIES PREFIX "")

target_include_directories(drcTB PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

set_target_properties(drcTB PROPERTIES PUBLIC_HEADER "${headers}")

install(TARGETS drcTB EXPORT drcTBTargets
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT shlib
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}" COMPONENT dev
)

install(TARGETS pydrcTB EXPORT drcTBTargets
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT shlib
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/libdrcTB_rdict.pcm"
  "${CMAKE_CURRENT_BINARY_DIR}/libdrcTB.rootmap"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)
