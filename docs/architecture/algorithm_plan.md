# 算法计划层

算法计划层用于把“选择了什么算法”推进到“每一轮通信怎么做”。

## 数据结构

- `AlgorithmPlan`：一个集合通信任务的完整计划。
- `PlanStage`：一轮通信阶段。
- `PlanStep`：一次 send、reduce、copy 等基本动作。

## 当前覆盖

- AllReduce：ring reduce-scatter + ring all-gather 计划。
- AllGather：ring gather 计划。
- ReduceScatter：ring scatter + reduce 计划。
- Broadcast：binomial tree 计划。
- AllToAll：pairwise exchange 计划。

## 后续映射

真实 HCCL 后端可以把 `PlanStep` 映射为底层 send/recv/reduce 操作，或者作为生成算法文档、调试日志和性能报告的统一中间表示。

## 导出计划

```bash
./scripts/dump_plan.sh all_reduce 1048576 8
```

输出 CSV 字段：

```text
algorithm,stage,op,src_rank,dst_rank,offset_bytes,bytes
```
