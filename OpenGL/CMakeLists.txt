# CMakeList.txt : CMake project for OpenGL, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.16)

function(my_add_executable)
  foreach(_source IN ITEMS ${ARGN})
    assign_source_group(${_source})
  endforeach()
  add_executable(${ARGV})
endfunction(my_add_executable)

file(GLOB_RECURSE ${PROJECT_NAME_U}_HEADER_FILES src/*.h *.hpp)
file(GLOB_RECURSE ${PROJECT_NAME_U}_SOURCE_FILES src/*.cpp)

# Add source to this project's executable.
my_add_executable(${PROJECT_NAME}
  ${${PROJECT_NAME_U}_HEADER_FILES}
  ${${PROJECT_NAME_U}_SOURCE_FILES}
)

target_precompile_headers(${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/hzpch.h
)

# TODO: Add tests and install targets if needed.
find_package(OpenGL REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
  spdlog::spdlog
  ${OPENGL_LIBRARIES}
  glfw
  Glad
  imgui
  stb_image
)