# 参考项目对标

## Ascend cann-hccl

链接：https://gitee.com/ascend/cann-hccl

用途：

- 对齐赛题要求和昇腾 HCCL/CANN 接口。
- 确认 CANN 版本、源码标签和编译方式。
- 后续真实后端必须以该项目的接口与约束为准。

我们当前对应模块：

- `include/cann_liberty/hccl_adapter.h`
- `include/cann_liberty/c_api.h`
- `docs/architecture/hccl_integration.md`

## NVIDIA NCCL

链接：https://github.com/NVIDIA/nccl

用途：

- 参考成熟集合通信库的工程结构、拓扑感知和性能测试方式。
- 学习 Ring、Tree、分层通信和网络插件抽象的组织方式。
- 对标 `nccl-tests` 的 benchmark 输出习惯。

我们当前对应模块：

- `benchmarks/policy_benchmark.cpp`
- `scripts/benchmark_local.sh`
- `include/cann_liberty/topology.h`
- `src/topology.cpp`

## NCCL Tests

链接：https://github.com/NVIDIA/nccl-tests

用途：

- 对标性能测试入口。
- 后续需要补充类似 `all_reduce_perf`、`all_gather_perf`、`reduce_scatter_perf` 的独立入口。
- 输出字段要逐步接近真实评测需要：数据规模、world size、算法名、耗时、带宽、校验结果。

我们当前对应模块：

- `benchmarks/policy_benchmark.cpp`

## MSCCL / MSCCL++

链接：

- https://github.com/microsoft/mscclpp
- https://github.com/microsoft/msccl

用途：

- 参考“算法计划中间表示”思想。
- 支撑比赛中 Agent 自动生成算法、计划和代码的创新叙事。
- 后续可以把 `AlgorithmPlan` 扩展为可搜索、可序列化、可优化的通信计划。

我们当前对应模块：

- `include/cann_liberty/algorithm_plan.h`
- `src/algorithm_plan.cpp`
- `tools/plan_dump.cpp`

## 当前差距

- 还没有真实 CANN/HCCL 编译环境。
- 还没有与官方 HCCL 插件 ABI 完全对齐。
- 还没有真实多卡、多机链路数据。
- 还没有 msprof 性能采集脚本。
- 算法计划还没有 lowering 到真实 send/recv/reduce 调用。

## 下一步对标动作

1. 用 cann-hccl 确认真实接口签名和编译方式。
2. 参考 nccl-tests 拆出单原语 benchmark。
3. 参考 MSCCL，把 `AlgorithmPlan` 导出为 JSON 或 YAML。
4. 在 Agent 目录记录每个算法计划的 Prompt、输出和测试结果。
