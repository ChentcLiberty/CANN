#pragma once

#include "cann_liberty/plugin.h"

#include <string>
#include <vector>

namespace cann_liberty {

enum class PlanOp {
  Send,
  Recv,
  Reduce,
  Copy,
};

struct PlanStep {
  PlanOp op;
  int src_rank;
  int dst_rank;
  std::size_t offset_bytes;
  std::size_t bytes;
};

struct PlanStage {
  std::string name;
  std::vector<PlanStep> steps;
};

struct AlgorithmPlan {
  std::string algorithm;
  CollectiveKind kind;
  int world_size;
  std::size_t bytes_per_rank;
  std::vector<PlanStage> stages;
};

AlgorithmPlan BuildAlgorithmPlan(const CollectiveRequest& request);
const char* PlanOpName(PlanOp op);
std::size_t CountPlanSteps(const AlgorithmPlan& plan);

}  // namespace cann_liberty
