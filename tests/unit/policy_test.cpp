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

void ExpectTopologyAlgorithm(cann_liberty::CollectiveKind kind,
                             std::size_t bytes,
                             int world_size,
                             const cann_liberty::Topology& topology,
                             const char* expected) {
  const cann_liberty::CollectiveRequest request{
      kind,
      cann_liberty::DataType::Float32,
      bytes,
      0,
      world_size,
  };

  const auto decision = cann_liberty::SelectAlgorithmDecision(request, topology);
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
  assert(cann_liberty::ClassifyTopology(topology) == cann_liberty::TopologyKind::FullMesh);
  assert(cann_liberty::AverageBandwidthGbps(topology) == 100.0);
  assert(cann_liberty::AverageLatencyUs(topology) == 1.0);

  const auto hierarchical = cann_liberty::BuildHierarchicalTopology(2, 8);
  assert(cann_liberty::ClassifyTopology(hierarchical) == cann_liberty::TopologyKind::Hierarchical);
  ExpectTopologyAlgorithm(cann_liberty::CollectiveKind::AllReduce,
                          1024 * 1024,
                          16,
                          hierarchical,
                          "hierarchical_fat_tree");

  cann_liberty::Topology sparse;
  sparse.world_size = 4;
  sparse.links.push_back({0, 1, "roce", 100.0, 6.0});
  sparse.links.push_back({1, 2, "roce", 100.0, 6.0});
  sparse.links.push_back({2, 3, "roce", 100.0, 6.0});
  assert(cann_liberty::ClassifyTopology(sparse) == cann_liberty::TopologyKind::Sparse);
  ExpectTopologyAlgorithm(cann_liberty::CollectiveKind::Broadcast,
                          1024 * 1024,
                          4,
                          sparse,
                          "adaptive_tree");

  return 0;
}
