#include "cann_liberty/algorithm_plan.h"

#include <cassert>
#include <cstring>

namespace {

cann_liberty::CollectiveRequest Request(cann_liberty::CollectiveKind kind,
                                        std::size_t bytes,
                                        int world_size) {
  return cann_liberty::CollectiveRequest{
      kind,
      cann_liberty::DataType::Float32,
      bytes,
      0,
      world_size,
  };
}

}  // namespace

int main() {
  const auto all_reduce = cann_liberty::BuildAlgorithmPlan(
      Request(cann_liberty::CollectiveKind::AllReduce, 1024 * 1024, 4));
  assert(all_reduce.stages.size() == 6);
  assert(cann_liberty::CountPlanSteps(all_reduce) == 36);
  assert(all_reduce.stages.front().steps.front().bytes == 256 * 1024);

  const auto all_gather = cann_liberty::BuildAlgorithmPlan(
      Request(cann_liberty::CollectiveKind::AllGather, 1024 * 1024, 4));
  assert(all_gather.stages.size() == 3);
  assert(cann_liberty::CountPlanSteps(all_gather) == 12);

  const auto reduce_scatter = cann_liberty::BuildAlgorithmPlan(
      Request(cann_liberty::CollectiveKind::ReduceScatter, 1024 * 1024, 4));
  assert(reduce_scatter.stages.size() == 3);
  assert(cann_liberty::CountPlanSteps(reduce_scatter) == 24);

  const auto broadcast = cann_liberty::BuildAlgorithmPlan(
      Request(cann_liberty::CollectiveKind::Broadcast, 1024 * 1024, 8));
  assert(broadcast.stages.size() == 3);
  assert(cann_liberty::CountPlanSteps(broadcast) == 14);
  assert(std::strcmp(cann_liberty::PlanOpName(broadcast.stages[0].steps[0].op), "send") == 0);

  const auto all_to_all = cann_liberty::BuildAlgorithmPlan(
      Request(cann_liberty::CollectiveKind::AllToAll, 1024 * 1024, 4));
  assert(all_to_all.stages.size() == 4);
  assert(cann_liberty::CountPlanSteps(all_to_all) == 16);

  return 0;
}
