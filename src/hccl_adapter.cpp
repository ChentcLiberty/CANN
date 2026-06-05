#include "cann_liberty/hccl_adapter.h"

namespace cann_liberty {

HcclAdapterStatus GetHcclAdapterStatus() {
#if defined(CANN_LIBERTY_ENABLE_HCCL)
  return HcclAdapterStatus{true, "HCCL adapter enabled at build time"};
#else
  return HcclAdapterStatus{false, "build without CANN_LIBERTY_ENABLE_HCCL"};
#endif
}

const char* ExpectedHcclEntryPoint(CollectiveKind kind) {
  switch (kind) {
    case CollectiveKind::AllReduce:
      return "hcclAllReduce";
    case CollectiveKind::AllGather:
      return "hcclAllGather";
    case CollectiveKind::ReduceScatter:
      return "hcclReduceScatter";
    case CollectiveKind::Broadcast:
      return "hcclBroadcast";
    case CollectiveKind::AllToAll:
      return "hcclAlltoAll";
  }
  return "unknown";
}

const char* ExpectedHcclDataType(DataType dtype) {
  switch (dtype) {
    case DataType::Float32:
      return "HCCL_DATA_TYPE_FP32";
    case DataType::Float16:
      return "HCCL_DATA_TYPE_FP16";
    case DataType::BFloat16:
      return "HCCL_DATA_TYPE_BFP16";
    case DataType::Int8:
      return "HCCL_DATA_TYPE_INT8";
    case DataType::Unknown:
      return "HCCL_DATA_TYPE_UNKNOWN";
  }
  return "HCCL_DATA_TYPE_UNKNOWN";
}

const char* ExpectedHcclReduceOp(ReduceOp op) {
  switch (op) {
    case ReduceOp::Sum:
      return "HCCL_REDUCE_SUM";
    case ReduceOp::Prod:
      return "HCCL_REDUCE_PROD";
    case ReduceOp::Max:
      return "HCCL_REDUCE_MAX";
    case ReduceOp::Min:
      return "HCCL_REDUCE_MIN";
    case ReduceOp::None:
      return "HCCL_REDUCE_NONE";
  }
  return "HCCL_REDUCE_NONE";
}

HcclCallResult ExecuteHcclCollective(const HcclCallRequest& request) {
  if (request.kind == CollectiveKind::AllReduce && request.reduce_op == ReduceOp::None) {
    return HcclCallResult{
        false,
        ExpectedHcclEntryPoint(request.kind),
        "AllReduce requires an explicit reduce op",
    };
  }

#if defined(CANN_LIBERTY_ENABLE_HCCL)
  return HcclCallResult{
      false,
      ExpectedHcclEntryPoint(request.kind),
      "real HCCL lowering is not implemented yet",
  };
#else
  return HcclCallResult{
      false,
      ExpectedHcclEntryPoint(request.kind),
      "HCCL adapter is unavailable in stub build",
  };
#endif
}

}  // namespace cann_liberty
