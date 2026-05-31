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
