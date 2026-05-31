# 赛题理解

本项目目标是围绕昇腾 CANN/HCCL 生态实现集合通信算法插件，并提交可复现的 Agent 工程。

## 核心评分面

- 通信原语正确性：至少覆盖 AllReduce、AllGather、ReduceScatter、Broadcast、AlltoAll 中三类。
- 通信性能：关注带宽、延迟、加速比、NPU 利用率。
- 拓扑适配：支持 HCCS、RoCE、PCIe，适配单机多卡和多机多卡。
- 可靠性：链路健康检测、动态路由切换、数据校验、超时重传、流量控制。
- Agent 交付：能力清单、Prompt、生成日志、运行说明、可复现工程。

## 初始技术路线

1. 先建立可编译插件骨架和本地模拟器。
2. 再实现拓扑抽象、算法选择器和基准算法。
3. 逐步补充 Butterfly、NHR、动态分块 Ring、Mesh、Fat-Tree。
4. 通过模拟器和真实 HCCL 环境分别验证正确性和性能。

