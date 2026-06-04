#!/usr/bin/env bash
set -euo pipefail

roots=()

if [[ -n "${ASCEND_HOME_PATH:-}" ]]; then
  roots+=("${ASCEND_HOME_PATH}")
fi

if [[ -n "${ASCEND_TOOLKIT_HOME:-}" ]]; then
  roots+=("${ASCEND_TOOLKIT_HOME}")
fi

roots+=(
  "/usr/local/Ascend/ascend-toolkit/latest"
  "/usr/local/Ascend/latest"
  "/usr/local/Ascend"
  "/opt/Ascend/ascend-toolkit/latest"
  "/opt/Ascend/latest"
  "/opt/Ascend"
)

echo "# CANN/HCCL Environment Report"
echo
echo "Generated at: $(date -u +%Y-%m-%dT%H:%M:%SZ)"
echo
echo "## Environment"
echo
echo "ASCEND_HOME_PATH=${ASCEND_HOME_PATH:-}"
echo "ASCEND_TOOLKIT_HOME=${ASCEND_TOOLKIT_HOME:-}"
echo "LD_LIBRARY_PATH=${LD_LIBRARY_PATH:-}"
echo
echo "## Candidate Roots"
echo

seen=""
for root in "${roots[@]}"; do
  if [[ "${seen}" == *"|${root}|"* ]]; then
    continue
  fi
  seen="${seen}|${root}|"
  if [[ -d "${root}" ]]; then
    echo "- found: ${root}"
  else
    echo "- missing: ${root}"
  fi
done

echo
echo "## Headers"
echo

header_patterns=(
  "hccl/hccl.h"
  "hccl.h"
  "hcom/hcom.h"
  "acl/acl.h"
)

for root in "${roots[@]}"; do
  [[ -d "${root}" ]] || continue
  for pattern in "${header_patterns[@]}"; do
    while IFS= read -r path; do
      [[ -n "${path}" ]] && echo "- ${path}"
    done < <(find "${root}" -path "*/include/*${pattern}" -type f 2>/dev/null | sort)
  done
done

echo
echo "## Libraries"
echo

lib_patterns=(
  "libhccl.so"
  "libhcom.so"
  "libascendcl.so"
)

for root in "${roots[@]}"; do
  [[ -d "${root}" ]] || continue
  for pattern in "${lib_patterns[@]}"; do
    while IFS= read -r path; do
      [[ -n "${path}" ]] && echo "- ${path}"
    done < <(find "${root}" -name "${pattern}" -type f 2>/dev/null | sort)
  done
done

echo
echo "## Suggested Next Step"
echo
echo "If headers and libraries are present, set CMake variables or environment paths for:"
echo
echo "- CANN include directory"
echo "- HCCL library directory"
echo "- runtime library path"
