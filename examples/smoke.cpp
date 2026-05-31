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

  std::cout << cann_liberty::SelectAlgorithm(request) << '\n';
  return 0;
}

