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

}  // namespace cann_liberty

