#pragma once

#include <vector>

namespace cann_liberty {

using RankBuffer = std::vector<float>;
using ClusterBuffers = std::vector<RankBuffer>;

ClusterBuffers SimulateAllReduceSum(const ClusterBuffers& input);
ClusterBuffers SimulateAllGather(const ClusterBuffers& input);
ClusterBuffers SimulateReduceScatterSum(const ClusterBuffers& input);
ClusterBuffers SimulateBroadcast(const ClusterBuffers& input, int root_rank);
ClusterBuffers SimulateAllToAll(const ClusterBuffers& input);

}  // namespace cann_liberty
