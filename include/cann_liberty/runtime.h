#pragma once

#include "cann_liberty/plugin.h"
#include "cann_liberty/simulator.h"

#include <string>

namespace cann_liberty {

enum class BackendKind {
  Simulator,
  Hccl,
};

struct RuntimeConfig {
  BackendKind backend;
  int root_rank;
};

struct CollectiveResult {
  ClusterBuffers buffers;
  AlgorithmDecision decision;
  std::string backend_name;
};

CollectiveResult ExecuteCollective(const RuntimeConfig& config,
                                   const CollectiveRequest& request,
                                   const ClusterBuffers& input);

const char* BackendKindName(BackendKind backend);

}  // namespace cann_liberty
