cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(lit VERSION 0.0.0)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a" ".so" ".lib" ".dll" ".dylib")
set(DEPS_DIR ${PROJECT_SOURCE_DIR}/deps)
set(GMP_DIR ${DEPS_DIR}/gmp)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE OLD_SOURCES "src/*.old.cpp")
list(REMOVE_ITEM SOURCES ${OLD_SOURCES})

include_directories(${GMP_DIR}/include)
include_directories("include")

find_library(GMP_LIBRARY gmp ${GMP_DIR}/lib)
find_library(GMPXX_LIBRARY gmpxx ${GMP_DIR}/lib)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${GMPXX_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${GMP_LIBRARY})

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-static" LINK_SEARCH_START_STATIC TRUE LINK_SEARCH_END_STATIC TRUE)
endif()

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
add_custom_target(uninstall
  COMMAND xargs rm < install_manifest.txt
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Uninstalling...")