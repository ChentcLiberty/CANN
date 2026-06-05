#pragma once

#include "cann_liberty/plugin.h"

#include <cstddef>

namespace cann_liberty {

struct HcclAdapterStatus {
  bool available;
  const char* reason;
};

struct HcclRuntimeHandles {
  void* communicator;
  void* stream;
};

struct HcclCallRequest {
  CollectiveKind kind;
  DataType dtype;
  ReduceOp reduce_op;
  HcclRuntimeHandles handles;
  const void* send_buffer;
  void* recv_buffer;
  std::size_t bytes;
  int root_rank;
  int rank;
  int world_size;
};

struct HcclCallResult {
  bool ok;
  const char* entry_point;
  const char* message;
};

HcclAdapterStatus GetHcclAdapterStatus();
bool HasUsableHcclHandles(const HcclRuntimeHandles& handles);
const char* ExpectedHcclEntryPoint(CollectiveKind kind);
const char* ExpectedHcclDataType(DataType dtype);
const char* ExpectedHcclReduceOp(ReduceOp op);
HcclCallResult ExecuteHcclCollective(const HcclCallRequest& request);

}  // namespace cann_liberty
