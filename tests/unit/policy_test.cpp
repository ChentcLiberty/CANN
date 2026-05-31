#include "cann_liberty/plugin.h"
#include "cann_liberty/topology.h"

#include <cassert>
#include <cstring>

namespace {

void ExpectAlgorithm(cann_liberty::CollectiveKind kind,
                     std::size_t bytes,
                     int world_size,
                     const char* expected) {
  const cann_liberty::CollectiveRequest request{
      kind,
      cann_liberty::DataType::Float32,
      bytes,
      0,
      world_size,
  };

  const auto decision = cann_liberty::SelectAlgorithmDecision(request);
  assert(std::strcmp(decision.name, expected) == 0);
  assert(decision.estimated_cost > 0.0);
}

}  // namespace

int main() {
  ExpectAlgorithm(cann_liberty::CollectiveKind::AllReduce, 4096, 8, "butterfly");
  ExpectAlgorithm(cann_liberty::CollectiveKind::AllToAll, 4096, 8, "pairwise");
  ExpectAlgorithm(cann_liberty::CollectiveKind::AllReduce, 1024 * 1024, 8, "nhr");
  ExpectAlgorithm(cann_liberty::CollectiveKind::Broadcast, 1024 * 1024, 8, "binomial_tree");
  ExpectAlgorithm(cann_liberty::CollectiveKind::AllGather, 1024 * 1024, 8, "dynamic_block_ring");
  ExpectAlgorithm(cann_liberty::CollectiveKind::AllReduce, 2ull * 1024ull * 1024ull * 1024ull, 8, "mesh");
  ExpectAlgorithm(cann_liberty::CollectiveKind::AllReduce, 2ull * 1024ull * 1024ull * 1024ull, 64, "hierarchical_fat_tree");

  const auto topology = cann_liberty::BuildFallbackFullMeshTopology(8);
  assert(cann_liberty::IsFullMesh(topology));
  assert(cann_liberty::AverageBandwidthGbps(topology) == 100.0);
  assert(cann_liberty::AverageLatencyUs(topology) == 1.0);

  return 0;
}
