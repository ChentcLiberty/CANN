#include "cann_liberty/plugin.h"

namespace cann_liberty {

const char* SelectAlgorithm(const CollectiveRequest& request) {
  if (request.bytes <= 64 * 1024) {
    return "butterfly";
  }

  if (request.bytes >= 1024ull * 1024ull * 1024ull) {
    return request.world_size > 8 ? "hierarchical_fat_tree" : "mesh";
  }

  switch (request.kind) {
    case CollectiveKind::AllReduce:
      return "nhr";
    case CollectiveKind::AllGather:
      return "dynamic_block_ring";
    case CollectiveKind::ReduceScatter:
      return "dynamic_block_ring";
    case CollectiveKind::Broadcast:
      return "binomial_tree";
    case CollectiveKind::AllToAll:
      return "pairwise";
  }

  return "ring";
}

}  // namespace cann_liberty

