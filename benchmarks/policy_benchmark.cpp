#include "cann_liberty/algorithm_plan.h"
#include "cann_liberty/runtime.h"

#include <chrono>
#include <cstddef>
#include <iostream>
#include <vector>

namespace {

cann_liberty::ClusterBuffers BuildInput(int world_size, std::size_t floats_per_rank) {
  cann_liberty::ClusterBuffers input;
  input.reserve(static_cast<std::size_t>(world_size));

  for (int rank = 0; rank < world_size; ++rank) {
    cann_liberty::RankBuffer buffer;
    buffer.reserve(floats_per_rank);
    for (std::size_t i = 0; i < floats_per_rank; ++i) {
      buffer.push_back(static_cast<float>(rank + 1));
    }
    input.push_back(buffer);
  }

  return input;
}

const char* KindName(cann_liberty::CollectiveKind kind) {
  switch (kind) {
    case cann_liberty::CollectiveKind::AllReduce:
      return "all_reduce";
    case cann_liberty::CollectiveKind::AllGather:
      return "all_gather";
    case cann_liberty::CollectiveKind::ReduceScatter:
      return "reduce_scatter";
    case cann_liberty::CollectiveKind::Broadcast:
      return "broadcast";
    case cann_liberty::CollectiveKind::AllToAll:
      return "all_to_all";
  }
  return "unknown";
}

}  // namespace

int main() {
  const std::vector<cann_liberty::CollectiveKind> kinds{
      cann_liberty::CollectiveKind::AllReduce,
      cann_liberty::CollectiveKind::AllGather,
      cann_liberty::CollectiveKind::ReduceScatter,
      cann_liberty::CollectiveKind::Broadcast,
      cann_liberty::CollectiveKind::AllToAll,
  };

  const std::vector<int> world_sizes{4, 8, 16};
  const std::vector<std::size_t> bytes_per_rank{
      4 * 1024,
      64 * 1024,
      1024 * 1024,
  };

  std::cout << "kind,world_size,bytes_per_rank,total_bytes,algorithm,backend,"
               "stage_count,step_count,elapsed_us,sim_mib_per_s\n";

  for (const int world_size : world_sizes) {
    for (const std::size_t bytes : bytes_per_rank) {
      const std::size_t floats_per_rank = bytes / sizeof(float);
      const auto input = BuildInput(world_size, floats_per_rank);

      for (const auto kind : kinds) {
        const cann_liberty::CollectiveRequest request{
            kind,
            cann_liberty::DataType::Float32,
            kind == cann_liberty::CollectiveKind::AllReduce ||
                    kind == cann_liberty::CollectiveKind::ReduceScatter
                ? cann_liberty::ReduceOp::Sum
                : cann_liberty::ReduceOp::None,
            bytes,
            0,
            world_size,
        };

        const auto start = std::chrono::steady_clock::now();
        const auto result = cann_liberty::ExecuteCollective(
            {cann_liberty::BackendKind::Simulator, 0},
            request,
            input);
        const auto stop = std::chrono::steady_clock::now();
        const auto elapsed_us =
            std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
        const auto plan = cann_liberty::BuildAlgorithmPlan(request);
        const std::size_t total_bytes = bytes * static_cast<std::size_t>(world_size);
        const double seconds = static_cast<double>(elapsed_us) / 1000000.0;
        const double mib_per_s =
            seconds > 0.0 ? (static_cast<double>(total_bytes) / (1024.0 * 1024.0)) / seconds
                          : 0.0;

        std::cout << KindName(kind) << ','
                  << world_size << ','
                  << bytes << ','
                  << total_bytes << ','
                  << result.decision.name << ','
                  << result.backend_name << ','
                  << plan.stages.size() << ','
                  << cann_liberty::CountPlanSteps(plan) << ','
                  << elapsed_us << ','
                  << mib_per_s << '\n';
      }
    }
  }

  return 0;
}
