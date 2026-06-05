#include "cann_liberty/hccl_adapter.h"
#include "cann_liberty/runtime.h"

#include <cassert>
#include <cstring>
#include <stdexcept>
#include <vector>

int main() {
  const cann_liberty::ClusterBuffers input{
      {1.0F, 2.0F, 3.0F, 4.0F},
      {10.0F, 20.0F, 30.0F, 40.0F},
      {100.0F, 200.0F, 300.0F, 400.0F},
      {1000.0F, 2000.0F, 3000.0F, 4000.0F},
  };

  const cann_liberty::CollectiveRequest request{
      cann_liberty::CollectiveKind::AllReduce,
      cann_liberty::DataType::Float32,
      cann_liberty::ReduceOp::Sum,
      4 * sizeof(float),
      0,
      4,
  };

  const cann_liberty::RuntimeConfig config{
      cann_liberty::BackendKind::Simulator,
      0,
  };

  const auto result = cann_liberty::ExecuteCollective(config, request, input);
  assert(std::strcmp(result.backend_name.c_str(), "simulator") == 0);
  assert(std::strcmp(result.decision.name, "butterfly") == 0);
  assert((result.buffers[0] == std::vector<float>{1111.0F, 2222.0F, 3333.0F, 4444.0F}));

  const auto status = cann_liberty::GetHcclAdapterStatus();
  assert(!status.available);
  assert(std::strcmp(cann_liberty::ExpectedHcclEntryPoint(cann_liberty::CollectiveKind::AllReduce),
                     "hcclAllReduce") == 0);
  assert(std::strcmp(cann_liberty::ExpectedHcclEntryPoint(cann_liberty::CollectiveKind::AllGather),
                     "hcclAllGather") == 0);
  assert(std::strcmp(cann_liberty::ExpectedHcclDataType(cann_liberty::DataType::Float32),
                     "HCCL_DATA_TYPE_FP32") == 0);
  assert(std::strcmp(cann_liberty::ExpectedHcclDataType(cann_liberty::DataType::BFloat16),
                     "HCCL_DATA_TYPE_BFP16") == 0);
  assert(std::strcmp(cann_liberty::ExpectedHcclReduceOp(cann_liberty::ReduceOp::Sum),
                     "HCCL_REDUCE_SUM") == 0);
  assert(std::strcmp(cann_liberty::ExpectedHcclReduceOp(cann_liberty::ReduceOp::None),
                     "HCCL_REDUCE_NONE") == 0);
  assert(!cann_liberty::HasUsableHcclHandles({nullptr, nullptr}));

  int fake_comm = 1;
  int fake_stream = 1;
  assert(cann_liberty::HasUsableHcclHandles({&fake_comm, &fake_stream}));

  const auto hccl_result = cann_liberty::ExecuteHcclCollective({
      cann_liberty::CollectiveKind::AllReduce,
      cann_liberty::DataType::Float32,
      cann_liberty::ReduceOp::Sum,
      {nullptr, nullptr},
      input[0].data(),
      nullptr,
      4 * sizeof(float),
      0,
      0,
      4,
  });
  assert(!hccl_result.ok);
  assert(std::strcmp(hccl_result.entry_point, "hcclAllReduce") == 0);

  bool threw = false;
  try {
    cann_liberty::ExecuteCollective({cann_liberty::BackendKind::Hccl, 0}, request, input);
  } catch (const std::runtime_error&) {
    threw = true;
  }
  assert(threw);

  return 0;
}
