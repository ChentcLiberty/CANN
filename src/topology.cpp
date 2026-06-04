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

Topology BuildHierarchicalTopology(int nodes, int ranks_per_node) {
  Topology topology;
  topology.world_size = nodes * ranks_per_node;

  for (int src = 0; src < topology.world_size; ++src) {
    for (int dst = 0; dst < topology.world_size; ++dst) {
      if (src == dst) {
        continue;
      }

      const bool same_node = (src / ranks_per_node) == (dst / ranks_per_node);
      topology.links.push_back(LinkInfo{
          src,
          dst,
          same_node ? "hccs" : "roce",
          same_node ? 240.0 : 100.0,
          same_node ? 0.8 : 6.0,
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

TopologyKind ClassifyTopology(const Topology& topology) {
  if (topology.world_size <= 0 || topology.links.empty()) {
    return TopologyKind::Empty;
  }

  if (!IsFullMesh(topology)) {
    return TopologyKind::Sparse;
  }

  bool has_multiple_link_types = false;
  const std::string& first_type = topology.links.front().type;
  for (const LinkInfo& link : topology.links) {
    if (link.type != first_type) {
      has_multiple_link_types = true;
      break;
    }
  }

  return has_multiple_link_types ? TopologyKind::Hierarchical : TopologyKind::FullMesh;
}

const char* TopologyKindName(TopologyKind kind) {
  switch (kind) {
    case TopologyKind::Empty:
      return "empty";
    case TopologyKind::FullMesh:
      return "full_mesh";
    case TopologyKind::Hierarchical:
      return "hierarchical";
    case TopologyKind::Sparse:
      return "sparse";
  }
  return "unknown";
}

}  // namespace cann_liberty
