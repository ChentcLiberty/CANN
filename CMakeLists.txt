cmake_minimum_required(VERSION 3.16)

project(CANNLiberty
  VERSION 0.1.0
  LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(CANN_LIBERTY_BUILD_TESTS "Build tests" ON)
option(CANN_LIBERTY_BUILD_EXAMPLES "Build examples" ON)

add_library(cann_liberty SHARED
  src/plugin.cpp
  src/topology.cpp
  src/algorithm_selector.cpp
)

target_include_directories(cann_liberty
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(cann_liberty
  PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

if(CANN_LIBERTY_BUILD_EXAMPLES)
  add_executable(cann_liberty_smoke examples/smoke.cpp)
  target_link_libraries(cann_liberty_smoke PRIVATE cann_liberty)
endif()

if(CANN_LIBERTY_BUILD_TESTS)
  enable_testing()
  add_executable(cann_liberty_policy_test tests/unit/policy_test.cpp)
  target_link_libraries(cann_liberty_policy_test PRIVATE cann_liberty)
  add_test(NAME cann_liberty_policy_test COMMAND cann_liberty_policy_test)
endif()
