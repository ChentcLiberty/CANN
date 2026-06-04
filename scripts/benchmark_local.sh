#!/usr/bin/env bash
set -euo pipefail

cxx="${CXX:-g++}"
out="/tmp/cann_liberty_policy_benchmark"

"${cxx}" -std=c++17 -O2 -Iinclude \
  src/plugin.cpp src/topology.cpp src/simulator.cpp src/runtime.cpp src/hccl_adapter.cpp \
  src/algorithm_plan.cpp \
  benchmarks/policy_benchmark.cpp \
  -o "${out}"

"${out}"
