cmake_minimum_required(VERSION 3.20)
project(CylinderExample LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(VTK REQUIRED)

add_executable(CylinderExample main.cpp)
target_link_libraries(CylinderExample PRIVATE ${VTK_LIBRARIES})

vtk_module_autoinit(
  TARGETS CylinderExample
  MODULES ${VTK_LIBRARIES}
)