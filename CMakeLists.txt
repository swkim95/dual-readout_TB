cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(dual-readout_TB)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

find_package(ROOT REQUIRED)

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
add_executable(read read.cc ${sources} ${headers})

target_include_directories(drcTB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${ROOT_INCLUDE_DIRS})

target_link_libraries(
  drcTB
  ${ROOT_LIBRARIES}
)

target_link_libraries(
  read
  ${ROOT_LIBRARIES}
)

target_include_directories(drcTB PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

set_target_properties(drcTB PROPERTIES PUBLIC_HEADER "${headers}")

install(TARGETS drcTB EXPORT drcTBTargets
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT shlib
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}" COMPONENT dev
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/libdrcTB_rdict.pcm"
  "${CMAKE_CURRENT_BINARY_DIR}/libdrcTB.rootmap"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)

install(TARGETS read EXPORT drcTBTargets
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT bin
)
