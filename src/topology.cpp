#include "cann_liberty/topology.h"

namespace cann_liberty {

Topology BuildFallbackFullMeshTopology(int world_size) {
  Topology topology;
  topology.world_size = world_size;

  for (int src = 0; src < world_size; ++src) {
    for (int dst = 0; dst < world_size; ++dst) {
      if (src == dst) {
        continue;
      }

      topology.links.push_back(LinkInfo{
          src,
          dst,
          "fallback_full_mesh",
          100.0,
          1.0,
      });
    }
  }

  return topology;
}

double AverageBandwidthGbps(const Topology& topology) {
  if (topology.links.empty()) {
    return 0.0;
  }

  double total = 0.0;
  for (const LinkInfo& link : topology.links) {
    total += link.bandwidth_gbps;
  }
  return total / static_cast<double>(topology.links.size());
}

double AverageLatencyUs(const Topology& topology) {
  if (topology.links.empty()) {
    return 0.0;
  }

  double total = 0.0;
  for (const LinkInfo& link : topology.links) {
    total += link.latency_us;
  }
  return total / static_cast<double>(topology.links.size());
}

bool IsFullMesh(const Topology& topology) {
  if (topology.world_size <= 1) {
    return true;
  }

  const int expected_links = topology.world_size * (topology.world_size - 1);
  return static_cast<int>(topology.links.size()) == expected_links;
}

}  // namespace cann_liberty
