#!/usr/bin/env bash
set -euo pipefail

cmake -S . -B build -DCANN_LIBERTY_BUILD_TESTS=ON
cmake --build build -j
./build/cann_liberty_smoke

