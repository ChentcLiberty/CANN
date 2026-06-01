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

}  // namespace cann_liberty
