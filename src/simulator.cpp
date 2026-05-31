#include "cann_liberty/simulator.h"

#include <stdexcept>

namespace cann_liberty {

namespace {

void RequireNonEmpty(const ClusterBuffers& input) {
  if (input.empty()) {
    throw std::invalid_argument("cluster input must not be empty");
  }
}

void RequireEqualRankBufferSize(const ClusterBuffers& input) {
  RequireNonEmpty(input);
  const std::size_t size = input.front().size();
  for (const RankBuffer& rank_buffer : input) {
    if (rank_buffer.size() != size) {
      throw std::invalid_argument("rank buffers must have equal size");
    }
  }
}

}  // namespace

ClusterBuffers SimulateAllReduceSum(const ClusterBuffers& input) {
  RequireEqualRankBufferSize(input);

  RankBuffer reduced(input.front().size(), 0.0F);
  for (const RankBuffer& rank_buffer : input) {
    for (std::size_t i = 0; i < rank_buffer.size(); ++i) {
      reduced[i] += rank_buffer[i];
    }
  }

  return ClusterBuffers(input.size(), reduced);
}

ClusterBuffers SimulateAllGather(const ClusterBuffers& input) {
  RequireNonEmpty(input);

  RankBuffer gathered;
  for (const RankBuffer& rank_buffer : input) {
    gathered.insert(gathered.end(), rank_buffer.begin(), rank_buffer.end());
  }

  return ClusterBuffers(input.size(), gathered);
}

ClusterBuffers SimulateReduceScatterSum(const ClusterBuffers& input) {
  RequireEqualRankBufferSize(input);
  if (input.front().size() % input.size() != 0) {
    throw std::invalid_argument("reduce-scatter buffer size must divide world size");
  }

  const ClusterBuffers reduced = SimulateAllReduceSum(input);
  const std::size_t chunk = input.front().size() / input.size();

  ClusterBuffers output;
  output.reserve(input.size());
  for (std::size_t rank = 0; rank < input.size(); ++rank) {
    const auto begin = reduced.front().begin() + static_cast<long>(rank * chunk);
    const auto end = begin + static_cast<long>(chunk);
    output.emplace_back(begin, end);
  }

  return output;
}

ClusterBuffers SimulateBroadcast(const ClusterBuffers& input, int root_rank) {
  RequireNonEmpty(input);
  if (root_rank < 0 || root_rank >= static_cast<int>(input.size())) {
    throw std::invalid_argument("broadcast root rank is out of range");
  }

  return ClusterBuffers(input.size(), input[static_cast<std::size_t>(root_rank)]);
}

ClusterBuffers SimulateAllToAll(const ClusterBuffers& input) {
  RequireEqualRankBufferSize(input);
  if (input.front().size() % input.size() != 0) {
    throw std::invalid_argument("all-to-all buffer size must divide world size");
  }

  const std::size_t world_size = input.size();
  const std::size_t chunk = input.front().size() / world_size;
  ClusterBuffers output(world_size);

  for (std::size_t dst = 0; dst < world_size; ++dst) {
    output[dst].reserve(input.front().size());
    for (std::size_t src = 0; src < world_size; ++src) {
      const auto begin = input[src].begin() + static_cast<long>(dst * chunk);
      const auto end = begin + static_cast<long>(chunk);
      output[dst].insert(output[dst].end(), begin, end);
    }
  }

  return output;
}

}  // namespace cann_liberty
