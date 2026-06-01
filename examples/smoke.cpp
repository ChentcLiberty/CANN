#include "cann_liberty/runtime.h"

#include <iostream>

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
      4 * sizeof(float),
      0,
      4,
  };

  const auto result = cann_liberty::ExecuteCollective(
      {cann_liberty::BackendKind::Simulator, 0},
      request,
      input);

  std::cout << result.backend_name << " " << result.decision.name << " ";
  for (float value : result.buffers.front()) {
    std::cout << value << " ";
  }
  std::cout << '\n';
  return 0;
}
