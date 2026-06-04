#!/usr/bin/env bash
set -euo pipefail

mkdir -p deliverables
"$(dirname "$0")/collect_artifacts.sh"
tar -czf deliverables/cann_liberty_source.tar.gz \
  README.md CMakeLists.txt include src examples tests benchmarks scripts docs agent configs simulator tools
