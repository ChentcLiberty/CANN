#!/usr/bin/env bash
set -euo pipefail

cxx="${CXX:-g++}"
out="/tmp/cann_liberty_plan_dump"
kind="${1:-all_reduce}"
bytes="${2:-1048576}"
world_size="${3:-8}"

"${cxx}" -std=c++17 -O2 -Iinclude \
  src/plugin.cpp src/topology.cpp src/algorithm_plan.cpp \
  tools/plan_dump.cpp \
  -o "${out}"

"${out}" "${kind}" "${bytes}" "${world_size}"
