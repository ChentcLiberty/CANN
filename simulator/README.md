# Simulator

用于在缺少真实昇腾集群时验证算法正确性、拓扑调度和可靠性机制。

## 已覆盖语义

- AllReduce Sum
- AllGather
- ReduceScatter Sum
- Broadcast
- AllToAll

对应实现位于 `include/cann_liberty/simulator.h` 和 `src/simulator.cpp`。
