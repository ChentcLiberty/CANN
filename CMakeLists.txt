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
option(CANN_LIBERTY_BUILD_BENCHMARKS "Build benchmarks" ON)
option(CANN_LIBERTY_ENABLE_HCCL "Enable real CANN/HCCL adapter" OFF)

set(CANN_ROOT "" CACHE PATH "Ascend CANN toolkit root")
set(HCCL_INCLUDE_DIR "" CACHE PATH "HCCL include directory")
set(HCCL_LIBRARY "" CACHE FILEPATH "HCCL shared library")

add_library(cann_liberty SHARED
  src/plugin.cpp
  src/topology.cpp
  src/algorithm_selector.cpp
  src/simulator.cpp
  src/runtime.cpp
  src/hccl_adapter.cpp
  src/c_api.cpp
  src/algorithm_plan.cpp
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

if(CANN_LIBERTY_ENABLE_HCCL)
  target_compile_definitions(cann_liberty PRIVATE CANN_LIBERTY_ENABLE_HCCL)

  if(CANN_ROOT AND NOT HCCL_INCLUDE_DIR)
    find_path(HCCL_INCLUDE_DIR
      NAMES hccl.h hccl/hccl.h
      PATHS ${CANN_ROOT}
      PATH_SUFFIXES include include/hccl
      NO_DEFAULT_PATH
    )
  endif()

  if(CANN_ROOT AND NOT HCCL_LIBRARY)
    find_library(HCCL_LIBRARY
      NAMES hccl
      PATHS ${CANN_ROOT}
      PATH_SUFFIXES lib64 lib
      NO_DEFAULT_PATH
    )
  endif()

  if(NOT HCCL_INCLUDE_DIR)
    message(FATAL_ERROR "CANN_LIBERTY_ENABLE_HCCL=ON requires HCCL_INCLUDE_DIR or CANN_ROOT")
  endif()

  if(NOT HCCL_LIBRARY)
    message(FATAL_ERROR "CANN_LIBERTY_ENABLE_HCCL=ON requires HCCL_LIBRARY or CANN_ROOT")
  endif()

  target_include_directories(cann_liberty PRIVATE ${HCCL_INCLUDE_DIR})
  target_link_libraries(cann_liberty PRIVATE ${HCCL_LIBRARY})
endif()

if(CANN_LIBERTY_BUILD_EXAMPLES)
  add_executable(cann_liberty_smoke examples/smoke.cpp)
  target_link_libraries(cann_liberty_smoke PRIVATE cann_liberty)

  add_executable(cann_liberty_collective_smoke examples/collective_smoke.cpp)
  target_link_libraries(cann_liberty_collective_smoke PRIVATE cann_liberty)
endif()

if(CANN_LIBERTY_BUILD_BENCHMARKS)
  add_executable(cann_liberty_policy_benchmark benchmarks/policy_benchmark.cpp)
  target_link_libraries(cann_liberty_policy_benchmark PRIVATE cann_liberty)
endif()

add_executable(cann_liberty_plan_dump tools/plan_dump.cpp)
target_link_libraries(cann_liberty_plan_dump PRIVATE cann_liberty)

if(CANN_LIBERTY_BUILD_TESTS)
  enable_testing()
  add_executable(cann_liberty_policy_test tests/unit/policy_test.cpp)
  target_link_libraries(cann_liberty_policy_test PRIVATE cann_liberty)
  add_test(NAME cann_liberty_policy_test COMMAND cann_liberty_policy_test)

  add_executable(cann_liberty_simulator_test tests/unit/simulator_test.cpp)
  target_link_libraries(cann_liberty_simulator_test PRIVATE cann_liberty)
  add_test(NAME cann_liberty_simulator_test COMMAND cann_liberty_simulator_test)

  add_executable(cann_liberty_runtime_test tests/unit/runtime_test.cpp)
  target_link_libraries(cann_liberty_runtime_test PRIVATE cann_liberty)
  add_test(NAME cann_liberty_runtime_test COMMAND cann_liberty_runtime_test)

  add_executable(cann_liberty_c_api_test tests/unit/c_api_test.cpp)
  target_link_libraries(cann_liberty_c_api_test PRIVATE cann_liberty)
  add_test(NAME cann_liberty_c_api_test COMMAND cann_liberty_c_api_test)

  add_executable(cann_liberty_algorithm_plan_test tests/unit/algorithm_plan_test.cpp)
  target_link_libraries(cann_liberty_algorithm_plan_test PRIVATE cann_liberty)
  add_test(NAME cann_liberty_algorithm_plan_test COMMAND cann_liberty_algorithm_plan_test)
endif()
