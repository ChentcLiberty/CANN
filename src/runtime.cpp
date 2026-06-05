#include "cann_liberty/runtime.h"

#include "cann_liberty/hccl_adapter.h"

#include <stdexcept>
#include <string>

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
    const void* send_buffer = nullptr;
    if (!input.empty() && !input.front().empty()) {
      send_buffer = input.front().data();
    }

    const HcclCallResult hccl_result = ExecuteHcclCollective({
        request.kind,
        request.dtype,
        send_buffer,
        nullptr,
        request.bytes,
        config.root_rank,
        request.rank,
        request.world_size,
    });

    if (!hccl_result.ok) {
      throw std::runtime_error(std::string(hccl_result.entry_point) + ": " + hccl_result.message);
    }
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
