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

}  // namespace cann_liberty

