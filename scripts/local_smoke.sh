#!/usr/bin/env bash
set -euo pipefail

cxx="${CXX:-g++}"
out="/tmp/cann_liberty_policy_test"

"${cxx}" -std=c++17 -Iinclude \
  src/plugin.cpp src/topology.cpp src/algorithm_selector.cpp \
  tests/unit/policy_test.cpp \
  -o "${out}"

"${out}"

