# 运行时后端设计

当前工程将集合通信执行拆成三层：

1. 策略层：根据通信原语、数据规模和集群规模选择算法。
2. 运行时层：统一调度 Simulator 或 HCCL 后端。
3. 后端层：Simulator 用 CPU 向量验证语义，HCCL 后端后续接入 CANN/HCCL API。

## 当前状态

- 默认可编译后端：Simulator
- 预留后端：HCCL
- HCCL 编译开关：`CANN_LIBERTY_ENABLE_HCCL`

未安装 CANN 的机器可以通过 `./scripts/local_smoke.sh` 验证策略、语义模拟和运行时调度。

