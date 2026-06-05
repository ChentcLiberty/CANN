#include "cann_liberty/algorithm_plan.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
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

void Usage(const char* argv0) {
  std::cerr << "usage: " << argv0 << " <kind> <bytes> <world_size>\n";
  std::cerr << "kind: all_reduce | all_gather | reduce_scatter | broadcast | all_to_all\n";
}

}  // namespace

int main(int argc, char** argv) {
  if (argc != 4) {
    Usage(argv[0]);
    return 2;
  }

  try {
    const cann_liberty::CollectiveRequest request{
        ParseKind(argv[1]),
        cann_liberty::DataType::Float32,
        argv[1] == std::string("all_reduce") || argv[1] == std::string("reduce_scatter")
            ? cann_liberty::ReduceOp::Sum
            : cann_liberty::ReduceOp::None,
        static_cast<std::size_t>(std::strtoull(argv[2], nullptr, 10)),
        0,
        std::atoi(argv[3]),
    };

    const auto plan = cann_liberty::BuildAlgorithmPlan(request);
    std::cout << "algorithm,stage,op,src_rank,dst_rank,offset_bytes,bytes\n";
    for (const auto& stage : plan.stages) {
      for (const auto& step : stage.steps) {
        std::cout << plan.algorithm << ','
                  << stage.name << ','
                  << cann_liberty::PlanOpName(step.op) << ','
                  << step.src_rank << ','
                  << step.dst_rank << ','
                  << step.offset_bytes << ','
                  << step.bytes << '\n';
      }
    }
  } catch (const std::exception& err) {
    std::cerr << err.what() << '\n';
    return 1;
  }

  return 0;
}
