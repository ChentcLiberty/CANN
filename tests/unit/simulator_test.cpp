#include "cann_liberty/simulator.h"

#include <cassert>
#include <vector>

int main() {
  const cann_liberty::ClusterBuffers input{
      {1.0F, 2.0F, 3.0F, 4.0F},
      {10.0F, 20.0F, 30.0F, 40.0F},
      {100.0F, 200.0F, 300.0F, 400.0F},
      {1000.0F, 2000.0F, 3000.0F, 4000.0F},
  };

  const auto all_reduce = cann_liberty::SimulateAllReduceSum(input);
  assert(all_reduce.size() == 4);
  assert((all_reduce[0] == std::vector<float>{1111.0F, 2222.0F, 3333.0F, 4444.0F}));
  assert(all_reduce[3] == all_reduce[0]);

  const auto all_gather = cann_liberty::SimulateAllGather(input);
  assert(all_gather.size() == 4);
  assert(all_gather[0].size() == 16);
  assert(all_gather[0][0] == 1.0F);
  assert(all_gather[0][15] == 4000.0F);

  const auto reduce_scatter = cann_liberty::SimulateReduceScatterSum(input);
  assert((reduce_scatter[0] == std::vector<float>{1111.0F}));
  assert((reduce_scatter[3] == std::vector<float>{4444.0F}));

  const auto broadcast = cann_liberty::SimulateBroadcast(input, 2);
  assert(broadcast.size() == 4);
  assert(broadcast[0] == input[2]);
  assert(broadcast[3] == input[2]);

  const auto all_to_all = cann_liberty::SimulateAllToAll(input);
  assert((all_to_all[0] == std::vector<float>{1.0F, 10.0F, 100.0F, 1000.0F}));
  assert((all_to_all[3] == std::vector<float>{4.0F, 40.0F, 400.0F, 4000.0F}));

  return 0;
}
