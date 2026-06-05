#include "cann_liberty/c_api.h"

#include "cann_liberty/hccl_adapter.h"
#include "cann_liberty/plugin.h"

namespace {

cann_liberty::CollectiveKind ToCollectiveKind(CannLibertyCollectiveKind kind) {
  switch (kind) {
    case CANN_LIBERTY_ALL_REDUCE:
      return cann_liberty::CollectiveKind::AllReduce;
    case CANN_LIBERTY_ALL_GATHER:
      return cann_liberty::CollectiveKind::AllGather;
    case CANN_LIBERTY_REDUCE_SCATTER:
      return cann_liberty::CollectiveKind::ReduceScatter;
    case CANN_LIBERTY_BROADCAST:
      return cann_liberty::CollectiveKind::Broadcast;
    case CANN_LIBERTY_ALL_TO_ALL:
      return cann_liberty::CollectiveKind::AllToAll;
  }
  return cann_liberty::CollectiveKind::AllReduce;
}

cann_liberty::DataType ToDataType(CannLibertyDataType dtype) {
  switch (dtype) {
    case CANN_LIBERTY_FLOAT32:
      return cann_liberty::DataType::Float32;
    case CANN_LIBERTY_FLOAT16:
      return cann_liberty::DataType::Float16;
    case CANN_LIBERTY_BFLOAT16:
      return cann_liberty::DataType::BFloat16;
    case CANN_LIBERTY_INT8:
      return cann_liberty::DataType::Int8;
    case CANN_LIBERTY_UNKNOWN:
      return cann_liberty::DataType::Unknown;
  }
  return cann_liberty::DataType::Unknown;
}

cann_liberty::ReduceOp ToReduceOp(CannLibertyReduceOp op) {
  switch (op) {
    case CANN_LIBERTY_REDUCE_SUM:
      return cann_liberty::ReduceOp::Sum;
    case CANN_LIBERTY_REDUCE_PROD:
      return cann_liberty::ReduceOp::Prod;
    case CANN_LIBERTY_REDUCE_MAX:
      return cann_liberty::ReduceOp::Max;
    case CANN_LIBERTY_REDUCE_MIN:
      return cann_liberty::ReduceOp::Min;
    case CANN_LIBERTY_REDUCE_NONE:
      return cann_liberty::ReduceOp::None;
  }
  return cann_liberty::ReduceOp::None;
}

}  // namespace

extern "C" CannLibertyDecision cann_liberty_select_algorithm(CannLibertyRequest request) {
  const cann_liberty::CollectiveRequest cpp_request{
      ToCollectiveKind(request.kind),
      ToDataType(request.dtype),
      ToReduceOp(request.reduce_op),
      request.bytes,
      request.rank,
      request.world_size,
  };

  const auto decision = cann_liberty::SelectAlgorithmDecision(cpp_request);
  return CannLibertyDecision{
      decision.name,
      decision.reason,
      decision.estimated_cost,
  };
}

extern "C" const char* cann_liberty_hccl_entry_point(CannLibertyCollectiveKind kind) {
  return cann_liberty::ExpectedHcclEntryPoint(ToCollectiveKind(kind));
}
