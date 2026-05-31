#pragma once

#include <cstddef>
#include <cstdint>

namespace cann_liberty {

enum class CollectiveKind {
  AllReduce,
  AllGather,
  ReduceScatter,
  Broadcast,
  AllToAll,
};

enum class DataType {
  Float32,
  Float16,
  BFloat16,
  Int8,
  Unknown,
};

struct CollectiveRequest {
  CollectiveKind kind;
  DataType dtype;
  std::size_t bytes;
  int rank;
  int world_size;
};

struct AlgorithmDecision {
  const char* name;
  const char* reason;
  double estimated_cost;
};

AlgorithmDecision SelectAlgorithmDecision(const CollectiveRequest& request);
const char* SelectAlgorithm(const CollectiveRequest& request);

}  // namespace cann_liberty
