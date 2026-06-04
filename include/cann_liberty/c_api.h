#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum CannLibertyCollectiveKind {
  CANN_LIBERTY_ALL_REDUCE = 0,
  CANN_LIBERTY_ALL_GATHER = 1,
  CANN_LIBERTY_REDUCE_SCATTER = 2,
  CANN_LIBERTY_BROADCAST = 3,
  CANN_LIBERTY_ALL_TO_ALL = 4,
} CannLibertyCollectiveKind;

typedef enum CannLibertyDataType {
  CANN_LIBERTY_FLOAT32 = 0,
  CANN_LIBERTY_FLOAT16 = 1,
  CANN_LIBERTY_BFLOAT16 = 2,
  CANN_LIBERTY_INT8 = 3,
  CANN_LIBERTY_UNKNOWN = 255,
} CannLibertyDataType;

typedef struct CannLibertyRequest {
  CannLibertyCollectiveKind kind;
  CannLibertyDataType dtype;
  size_t bytes;
  int rank;
  int world_size;
} CannLibertyRequest;

typedef struct CannLibertyDecision {
  const char* algorithm;
  const char* reason;
  double estimated_cost;
} CannLibertyDecision;

CannLibertyDecision cann_liberty_select_algorithm(CannLibertyRequest request);
const char* cann_liberty_hccl_entry_point(CannLibertyCollectiveKind kind);

#ifdef __cplusplus
}
#endif
