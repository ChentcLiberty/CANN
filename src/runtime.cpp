#include "cann_liberty/runtime.h"

#include <stdexcept>

namespace cann_liberty {

const char* BackendKindName(BackendKind backend) {
  switch (backend) {
    case BackendKind::Simulator:
      return "simulator";
    case BackendKind::Hccl:
      return "hccl";
  }
  return "unknown";
}

CollectiveResult ExecuteCollective(const RuntimeConfig& config,
                                   const CollectiveRequest& request,
                                   const ClusterBuffers& input) {
  const AlgorithmDecision decision = SelectAlgorithmDecision(request);

  if (config.backend == BackendKind::Hccl) {
    throw std::runtime_error("HCCL backend is not linked in this scaffold build");
  }

  ClusterBuffers output;
  switch (request.kind) {
    case CollectiveKind::AllReduce:
      output = SimulateAllReduceSum(input);
      break;
    case CollectiveKind::AllGather:
      output = SimulateAllGather(input);
      break;
    case CollectiveKind::ReduceScatter:
      output = SimulateReduceScatterSum(input);
      break;
    case CollectiveKind::Broadcast:
      output = SimulateBroadcast(input, config.root_rank);
      break;
    case CollectiveKind::AllToAll:
      output = SimulateAllToAll(input);
      break;
  }

  return CollectiveResult{
      output,
      decision,
      BackendKindName(config.backend),
  };
}

}  // namespace cann_liberty
