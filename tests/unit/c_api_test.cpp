#include "cann_liberty/c_api.h"

#include <cassert>
#include <cstring>

int main() {
  const CannLibertyRequest request{
      CANN_LIBERTY_ALL_REDUCE,
      CANN_LIBERTY_FLOAT32,
      CANN_LIBERTY_REDUCE_SUM,
      1024 * 1024,
      0,
      8,
  };

  const CannLibertyDecision decision = cann_liberty_select_algorithm(request);
  assert(std::strcmp(decision.algorithm, "nhr") == 0);
  assert(decision.estimated_cost > 0.0);

  assert(std::strcmp(cann_liberty_hccl_entry_point(CANN_LIBERTY_ALL_REDUCE),
                     "hcclAllReduce") == 0);
  assert(std::strcmp(cann_liberty_hccl_entry_point(CANN_LIBERTY_ALL_GATHER),
                     "hcclAllGather") == 0);

  return 0;
}
