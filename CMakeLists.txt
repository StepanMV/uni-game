cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 17)
project(TerrariaBossRush VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Raylib install

set(RAYLIB_VERSION 4.5.0)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
  include(FetchContent)
  FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(raylib)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
  endif()
endif()

# Executable

file(GLOB_RECURSE sources src/*.cpp src/*.h)
file(GLOB_RECURSE sources_test tests/*.cpp tests/*.h tests/*.c)
file(GLOB_RECURSE data resources/*)

add_executable(game ${sources} ${data})
add_executable(test ${sources_test})
target_link_libraries(game raylib)
target_link_libraries(test raylib)
target_include_directories(game PUBLIC ${CMAKE_SOURCE_DIR}/src/include)

file(COPY resources DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

file(GLOB_RECURSE extra extra/*)
file(COPY ${extra} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})