#pragma once

#include <string>
#include <vector>

namespace cann_liberty {

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
double AverageBandwidthGbps(const Topology& topology);
double AverageLatencyUs(const Topology& topology);
bool IsFullMesh(const Topology& topology);

}  // namespace cann_liberty
