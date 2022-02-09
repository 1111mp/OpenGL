# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.16)

project ("OpenGL")

set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

if(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
  add_compile_definitions(HZ_ENABLE_ASSERTS)
endif()

configure_file (
  "${PROJECT_SOURCE_DIR}/Config.h.in"
  "${PROJECT_SOURCE_DIR}/OpenGL/src/Config.h"
)

function(assign_source_group)
  foreach(_source IN ITEMS ${ARGN})
    if (IS_ABSOLUTE "${_source}")
      file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
    else()
      set(_source_rel "${_source}")
    endif()
    get_filename_component(_source_path "${_source_rel}" PATH)
    string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
    source_group("${_source_path_msvc}" FILES "${_source}")
  endforeach()
endfunction(assign_source_group)

include_directories(
  "Thirdparty/Glad/include"
  "Thirdparty/spdlog/include"
  "Thirdparty/imgui"
  "Thirdparty/glm"
  "Thirdparty/stb_image"
)

# Include sub-projects.
add_subdirectory("Thirdparty")
add_subdirectory ("OpenGL")
