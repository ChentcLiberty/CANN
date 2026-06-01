#pragma once

#include "cann_liberty/plugin.h"

namespace cann_liberty {

struct HcclAdapterStatus {
  bool available;
  const char* reason;
};

HcclAdapterStatus GetHcclAdapterStatus();
const char* ExpectedHcclEntryPoint(CollectiveKind kind);

}  // namespace cann_liberty
