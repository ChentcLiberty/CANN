#!/usr/bin/env bash
set -euo pipefail

cxx="${CXX:-g++}"
out="/tmp/cann_liberty_collective_smoke"
backend="${1:-simulator}"
kind="${2:-all_reduce}"
bytes="${3:-1048576}"
world_size="${4:-8}"

"${cxx}" -std=c++17 -O2 -Iinclude \
  src/plugin.cpp src/topology.cpp src/simulator.cpp src/runtime.cpp src/hccl_adapter.cpp \
  examples/collective_smoke.cpp \
  -o "${out}"

"${out}" "${backend}" "${kind}" "${bytes}" "${world_size}"
