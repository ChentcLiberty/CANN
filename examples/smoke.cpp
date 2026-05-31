#include "cann_liberty/plugin.h"

#include <iostream>

int main() {
  const cann_liberty::CollectiveRequest request{
      cann_liberty::CollectiveKind::AllReduce,
      cann_liberty::DataType::Float32,
      1024 * 1024,
      0,
      8,
  };

  const auto decision = cann_liberty::SelectAlgorithmDecision(request);
  std::cout << decision.name << ": " << decision.reason << '\n';
  return 0;
}
