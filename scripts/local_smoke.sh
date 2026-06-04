#!/usr/bin/env bash
set -euo pipefail

cxx="${CXX:-g++}"
out="/tmp/cann_liberty_policy_test"

"${cxx}" -std=c++17 -Iinclude \
  src/plugin.cpp src/topology.cpp src/algorithm_selector.cpp \
  tests/unit/policy_test.cpp \
  -o "${out}"

"${out}"

out="/tmp/cann_liberty_simulator_test"

"${cxx}" -std=c++17 -Iinclude \
  src/simulator.cpp \
  tests/unit/simulator_test.cpp \
  -o "${out}"

"${out}"

out="/tmp/cann_liberty_runtime_test"

"${cxx}" -std=c++17 -Iinclude \
  src/plugin.cpp src/topology.cpp src/simulator.cpp src/runtime.cpp src/hccl_adapter.cpp \
  tests/unit/runtime_test.cpp \
  -o "${out}"

"${out}"

out="/tmp/cann_liberty_c_api_test"

"${cxx}" -std=c++17 -Iinclude \
  src/plugin.cpp src/topology.cpp src/hccl_adapter.cpp src/c_api.cpp \
  tests/unit/c_api_test.cpp \
  -o "${out}"

"${out}"

out="/tmp/cann_liberty_algorithm_plan_test"

"${cxx}" -std=c++17 -Iinclude \
  src/plugin.cpp src/topology.cpp src/algorithm_plan.cpp \
  tests/unit/algorithm_plan_test.cpp \
  -o "${out}"

"${out}"
