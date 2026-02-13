cmake_minimum_required(VERSION 3.20)

# Project
project(worksheet5_calc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ===== Library: maths (adder) =====
add_library(maths adder.cpp)
target_include_directories(maths PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# ===== Executable: calc =====
add_executable(calc calc.cpp)
target_link_libraries(calc PRIVATE maths)

# ===== Install rules (THIS is what CPack packages) =====
install(TARGETS calc
        RUNTIME DESTINATION bin)

install(TARGETS maths
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)

install(FILES adder.h
        DESTINATION include)

# ===== CPack metadata =====
set(CPACK_PACKAGE_NAME "worksheet5-calc")
set(CPACK_PACKAGE_VENDOR "UoN")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_CONTACT "student@example.com")

# If you have NSIS installed, this makes a Windows installer (.exe).
# If not installed, it will fail; you can switch to ZIP (see Step 4).
if(WIN32)
  set(CPACK_GENERATOR "NSIS")
  set(CPACK_NSIS_DISPLAY_NAME "Worksheet5 Calc")
  set(CPACK_NSIS_PACKAGE_NAME "Worksheet5 Calc")
  set(CPACK_NSIS_MODIFY_PATH OFF)
endif()

include(CPack)
