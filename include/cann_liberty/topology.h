#pragma once

#include <string>
#include <vector>

namespace cann_liberty {

enum class TopologyKind {
  Empty,
  FullMesh,
  Hierarchical,
  Sparse,
};

struct LinkInfo {
  int src_rank;
  int dst_rank;
  std::string type;
  double bandwidth_gbps;
  double latency_us;
};

struct Topology {
  int world_size;
  std::vector<LinkInfo> links;
};

Topology BuildFallbackFullMeshTopology(int world_size);
Topology BuildHierarchicalTopology(int nodes, int ranks_per_node);
double AverageBandwidthGbps(const Topology& topology);
double AverageLatencyUs(const Topology& topology);
bool IsFullMesh(const Topology& topology);
TopologyKind ClassifyTopology(const Topology& topology);
const char* TopologyKindName(TopologyKind kind);

}  // namespace cann_liberty
