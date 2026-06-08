#include "cann_liberty/runtime.h"

#include <algorithm>
#include <cstdlib>
#include <exception>
#include <cmath>
#include <iostream>
#include <limits>
#include <string>

namespace {

cann_liberty::CollectiveKind ParseKind(const std::string& value) {
  if (value == "all_reduce") {
    return cann_liberty::CollectiveKind::AllReduce;
  }
  if (value == "all_gather") {
    return cann_liberty::CollectiveKind::AllGather;
  }
  if (value == "reduce_scatter") {
    return cann_liberty::CollectiveKind::ReduceScatter;
  }
  if (value == "broadcast") {
    return cann_liberty::CollectiveKind::Broadcast;
  }
  if (value == "all_to_all") {
    return cann_liberty::CollectiveKind::AllToAll;
  }
  throw std::invalid_argument("unknown collective kind: " + value);
}

cann_liberty::BackendKind ParseBackend(const std::string& value) {
  if (value == "simulator") {
    return cann_liberty::BackendKind::Simulator;
  }
  if (value == "hccl") {
    return cann_liberty::BackendKind::Hccl;
  }
  throw std::invalid_argument("unknown backend: " + value);
}

cann_liberty::ReduceOp ReduceOpForKind(cann_liberty::CollectiveKind kind) {
  return kind == cann_liberty::CollectiveKind::AllReduce ||
                 kind == cann_liberty::CollectiveKind::ReduceScatter
             ? cann_liberty::ReduceOp::Sum
             : cann_liberty::ReduceOp::None;
}

cann_liberty::ClusterBuffers BuildInput(int world_size, std::size_t bytes_per_rank) {
  const std::size_t floats_per_rank = bytes_per_rank / sizeof(float);
  cann_liberty::ClusterBuffers input;
  input.reserve(static_cast<std::size_t>(world_size));

  for (int rank = 0; rank < world_size; ++rank) {
    cann_liberty::RankBuffer buffer(floats_per_rank, static_cast<float>(rank + 1));
    input.push_back(buffer);
  }

  return input;
}

cann_liberty::ClusterBuffers ExpectedOutput(cann_liberty::CollectiveKind kind,
                                            const cann_liberty::ClusterBuffers& input) {
  switch (kind) {
    case cann_liberty::CollectiveKind::AllReduce:
      return cann_liberty::SimulateAllReduceSum(input);
    case cann_liberty::CollectiveKind::AllGather:
      return cann_liberty::SimulateAllGather(input);
    case cann_liberty::CollectiveKind::ReduceScatter:
      return cann_liberty::SimulateReduceScatterSum(input);
    case cann_liberty::CollectiveKind::Broadcast:
      return cann_liberty::SimulateBroadcast(input, 0);
    case cann_liberty::CollectiveKind::AllToAll:
      return cann_liberty::SimulateAllToAll(input);
  }
  return {};
}

double MaxAbsError(const cann_liberty::ClusterBuffers& actual,
                   const cann_liberty::ClusterBuffers& expected) {
  if (actual.size() != expected.size()) {
    return std::numeric_limits<double>::infinity();
  }

  double max_error = 0.0;
  for (std::size_t rank = 0; rank < actual.size(); ++rank) {
    if (actual[rank].size() != expected[rank].size()) {
      return std::numeric_limits<double>::infinity();
    }
    for (std::size_t i = 0; i < actual[rank].size(); ++i) {
      max_error =
          std::max(max_error, std::abs(static_cast<double>(actual[rank][i] - expected[rank][i])));
    }
  }
  return max_error;
}

void Usage(const char* argv0) {
  std::cerr << "usage: " << argv0 << " <backend> <kind> <bytes_per_rank> <world_size>\n";
  std::cerr << "backend: simulator | hccl\n";
  std::cerr << "kind: all_reduce | all_gather | reduce_scatter | broadcast | all_to_all\n";
}

}  // namespace

int main(int argc, char** argv) {
  if (argc != 5) {
    Usage(argv[0]);
    return 2;
  }

  try {
    const cann_liberty::BackendKind backend = ParseBackend(argv[1]);
    const cann_liberty::CollectiveKind kind = ParseKind(argv[2]);
    const std::size_t bytes = static_cast<std::size_t>(std::strtoull(argv[3], nullptr, 10));
    const int world_size = std::atoi(argv[4]);
    const auto input = BuildInput(world_size, bytes);

    const cann_liberty::CollectiveRequest request{
        kind,
        cann_liberty::DataType::Float32,
        ReduceOpForKind(kind),
        bytes,
        0,
        world_size,
    };

    const auto result = cann_liberty::ExecuteCollective({backend, 0}, request, input);
    const auto expected = ExpectedOutput(kind, input);
    const double max_abs_error = MaxAbsError(result.buffers, expected);
    const bool passed = max_abs_error == 0.0;

    std::cout << "backend=" << result.backend_name
              << ",algorithm=" << result.decision.name
              << ",output_ranks=" << result.buffers.size();
    if (!result.buffers.empty()) {
      std::cout << ",rank0_values=" << result.buffers.front().size();
    }
    std::cout << ",max_abs_error=" << max_abs_error
              << ",check=" << (passed ? "pass" : "fail") << '\n';

    if (!passed) {
      return 1;
    }
  } catch (const std::exception& err) {
    std::cerr << "collective_smoke failed: " << err.what() << '\n';
    return 1;
  }

  return 0;
}
