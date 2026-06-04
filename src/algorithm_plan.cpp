#include "cann_liberty/algorithm_plan.h"

#include <algorithm>
#include <stdexcept>

namespace cann_liberty {

namespace {

int PositiveWorldSize(const CollectiveRequest& request) {
  if (request.world_size <= 0) {
    throw std::invalid_argument("world size must be positive");
  }
  return request.world_size;
}

std::size_t ChunkBytes(const CollectiveRequest& request) {
  const int world_size = PositiveWorldSize(request);
  return std::max<std::size_t>(1, request.bytes / static_cast<std::size_t>(world_size));
}

void AddRingExchange(PlanStage* stage,
                     int world_size,
                     std::size_t chunk_bytes,
                     int round) {
  for (int rank = 0; rank < world_size; ++rank) {
    const int dst = (rank + 1) % world_size;
    const int chunk = (rank - round + world_size) % world_size;
    stage->steps.push_back(PlanStep{
        PlanOp::Send,
        rank,
        dst,
        static_cast<std::size_t>(chunk) * chunk_bytes,
        chunk_bytes,
    });
  }
}

AlgorithmPlan BuildAllReducePlan(const CollectiveRequest& request) {
  const int world_size = PositiveWorldSize(request);
  const std::size_t chunk_bytes = ChunkBytes(request);
  AlgorithmPlan plan{
      SelectAlgorithm(request),
      request.kind,
      world_size,
      request.bytes,
      {},
  };

  for (int round = 0; round < world_size - 1; ++round) {
    PlanStage stage{"reduce_scatter_round_" + std::to_string(round), {}};
    AddRingExchange(&stage, world_size, chunk_bytes, round);
    for (int rank = 0; rank < world_size; ++rank) {
      stage.steps.push_back(PlanStep{
          PlanOp::Reduce,
          (rank - 1 + world_size) % world_size,
          rank,
          static_cast<std::size_t>((rank - round - 1 + world_size) % world_size) * chunk_bytes,
          chunk_bytes,
      });
    }
    plan.stages.push_back(stage);
  }

  for (int round = 0; round < world_size - 1; ++round) {
    PlanStage stage{"all_gather_round_" + std::to_string(round), {}};
    AddRingExchange(&stage, world_size, chunk_bytes, round);
    plan.stages.push_back(stage);
  }

  return plan;
}

AlgorithmPlan BuildGatherScatterPlan(const CollectiveRequest& request) {
  const int world_size = PositiveWorldSize(request);
  const std::size_t chunk_bytes = ChunkBytes(request);
  AlgorithmPlan plan{
      SelectAlgorithm(request),
      request.kind,
      world_size,
      request.bytes,
      {},
  };

  const int rounds = world_size - 1;
  for (int round = 0; round < rounds; ++round) {
    PlanStage stage{std::string(request.kind == CollectiveKind::AllGather ? "gather_round_"
                                                                          : "scatter_round_") +
                        std::to_string(round),
                    {}};
    AddRingExchange(&stage, world_size, chunk_bytes, round);
    if (request.kind == CollectiveKind::ReduceScatter) {
      for (int rank = 0; rank < world_size; ++rank) {
        stage.steps.push_back(PlanStep{
            PlanOp::Reduce,
            (rank - 1 + world_size) % world_size,
            rank,
            static_cast<std::size_t>((rank - round - 1 + world_size) % world_size) * chunk_bytes,
            chunk_bytes,
        });
      }
    }
    plan.stages.push_back(stage);
  }

  return plan;
}

AlgorithmPlan BuildBroadcastPlan(const CollectiveRequest& request) {
  const int world_size = PositiveWorldSize(request);
  AlgorithmPlan plan{
      SelectAlgorithm(request),
      request.kind,
      world_size,
      request.bytes,
      {},
  };

  int distance = 1;
  int round = 0;
  while (distance < world_size) {
    PlanStage stage{"tree_round_" + std::to_string(round), {}};
    for (int src = 0; src < world_size; src += distance * 2) {
      const int dst = src + distance;
      if (dst < world_size) {
        stage.steps.push_back(PlanStep{PlanOp::Send, src, dst, 0, request.bytes});
        stage.steps.push_back(PlanStep{PlanOp::Copy, src, dst, 0, request.bytes});
      }
    }
    plan.stages.push_back(stage);
    distance *= 2;
    ++round;
  }

  return plan;
}

AlgorithmPlan BuildAllToAllPlan(const CollectiveRequest& request) {
  const int world_size = PositiveWorldSize(request);
  const std::size_t chunk_bytes = ChunkBytes(request);
  AlgorithmPlan plan{
      SelectAlgorithm(request),
      request.kind,
      world_size,
      request.bytes,
      {},
  };

  for (int round = 0; round < world_size; ++round) {
    PlanStage stage{"pairwise_round_" + std::to_string(round), {}};
    for (int rank = 0; rank < world_size; ++rank) {
      const int dst = (rank + round) % world_size;
      stage.steps.push_back(PlanStep{
          dst == rank ? PlanOp::Copy : PlanOp::Send,
          rank,
          dst,
          static_cast<std::size_t>(dst) * chunk_bytes,
          chunk_bytes,
      });
    }
    plan.stages.push_back(stage);
  }

  return plan;
}

}  // namespace

AlgorithmPlan BuildAlgorithmPlan(const CollectiveRequest& request) {
  switch (request.kind) {
    case CollectiveKind::AllReduce:
      return BuildAllReducePlan(request);
    case CollectiveKind::AllGather:
    case CollectiveKind::ReduceScatter:
      return BuildGatherScatterPlan(request);
    case CollectiveKind::Broadcast:
      return BuildBroadcastPlan(request);
    case CollectiveKind::AllToAll:
      return BuildAllToAllPlan(request);
  }
  throw std::invalid_argument("unsupported collective kind");
}

const char* PlanOpName(PlanOp op) {
  switch (op) {
    case PlanOp::Send:
      return "send";
    case PlanOp::Recv:
      return "recv";
    case PlanOp::Reduce:
      return "reduce";
    case PlanOp::Copy:
      return "copy";
  }
  return "unknown";
}

std::size_t CountPlanSteps(const AlgorithmPlan& plan) {
  std::size_t total = 0;
  for (const PlanStage& stage : plan.stages) {
    total += stage.steps.size();
  }
  return total;
}

}  // namespace cann_liberty
