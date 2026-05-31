#include "cann_liberty/plugin.h"

namespace cann_liberty {

namespace {

constexpr std::size_t kSmallMessageBytes = 64 * 1024;
constexpr std::size_t kLargeMessageBytes = 1024ull * 1024ull * 1024ull;

double CostByMessageAndScale(const CollectiveRequest& request, double factor) {
  const double kb = static_cast<double>(request.bytes) / 1024.0;
  const double ranks = request.world_size > 0 ? request.world_size : 1;
  return kb * ranks * factor;
}

}  // namespace

AlgorithmDecision SelectAlgorithmDecision(const CollectiveRequest& request) {
  if (request.bytes <= 64 * 1024) {
    if (request.kind == CollectiveKind::AllToAll) {
      return {"pairwise", "small all-to-all favors direct pair exchange", 1.0};
    }
    return {"butterfly", "small message favors logarithmic fanout", 1.0};
  }

  if (request.bytes >= kLargeMessageBytes) {
    if (request.world_size > 8) {
      return {"hierarchical_fat_tree",
              "large multi-node message favors hierarchy-aware scheduling",
              CostByMessageAndScale(request, 0.75)};
    }
    return {"mesh",
            "large single-node message favors full-mesh bandwidth",
            CostByMessageAndScale(request, 0.9)};
  }

  switch (request.kind) {
    case CollectiveKind::AllReduce:
      return {"nhr",
              "medium all-reduce uses non-uniform hierarchical ring",
              CostByMessageAndScale(request, 1.0)};
    case CollectiveKind::AllGather:
      return {"dynamic_block_ring",
              "all-gather benefits from adaptive chunking",
              CostByMessageAndScale(request, 1.1)};
    case CollectiveKind::ReduceScatter:
      return {"dynamic_block_ring",
              "reduce-scatter benefits from adaptive chunking",
              CostByMessageAndScale(request, 1.1)};
    case CollectiveKind::Broadcast:
      return {"binomial_tree",
              "broadcast maps naturally to tree fanout",
              CostByMessageAndScale(request, 0.8)};
    case CollectiveKind::AllToAll:
      return {"pairwise",
              "all-to-all requires deterministic rank pair exchange",
              CostByMessageAndScale(request, 1.3)};
  }

  return {"ring", "fallback baseline", CostByMessageAndScale(request, 1.5)};
}

const char* SelectAlgorithm(const CollectiveRequest& request) {
  return SelectAlgorithmDecision(request).name;
}

}  // namespace cann_liberty
