# 比赛推进路线

## 当前已经完成

- 工程骨架：CMake、头文件、源码、测试、benchmark、文档、Agent 目录。
- GitHub：已推送到 `ChentcLiberty/CANN`。
- 策略层：按通信原语、数据规模、world size 选择基础算法。
- 模拟器：CPU 验证 AllReduce、AllGather、ReduceScatter、Broadcast、AllToAll 语义。
- 运行时：Simulator/HCCL 后端抽象，当前默认 Simulator。
- Benchmark：可输出 CSV 作为后续性能报告起点。
- 算法计划层：可生成 AllReduce、AllGather、ReduceScatter、Broadcast、AllToAll 的通信阶段和步骤。
- 参考项目对标：已记录 cann-hccl、NCCL、nccl-tests、MSCCL/MSCCL++ 的借鉴点。
- 产物归档：可一键生成 smoke 日志、benchmark CSV 和核心算法计划 CSV。
- CANN/HCCL 环境探测：可扫描 Ascend Toolkit 头文件和动态库。
- HCCL lowering 边界：已建立 collective、dtype、reduce-op 映射占位。
- HCCL runtime handles：已建立 communicator/stream opaque 句柄占位。
- 单原语 smoke：已建立 simulator/hccl 共用 CLI 入口。

## 最急需完成

1. HCCL/CANN 环境接入
   - 找到 CANN 8.0+ 安装路径。
   - 确认 HCCL 头文件、动态库、插件 ABI 和样例编译命令。
   - 形成 `docs/architecture/hccl_integration.md`。

2. 真实插件入口
   - 建立与 HCCL 插件接口一致的 C ABI。
   - 将当前 `Runtime` 和 `AlgorithmDecision` 接到插件入口。
   - 保证无 CANN 环境时 Simulator 仍可编译。
   - 当前已有 `include/cann_liberty/c_api.h` 作为 C ABI 占位，下一步需要对齐官方 HCCL 插件 ABI。

3. 至少 3 个可解释算法
   - 优先级：AllReduce、AllGather、ReduceScatter。
   - 每个算法需要：流程图、复杂度、拓扑适配点、Simulator 对照测试。

4. 验证闭环
   - 正确性：与 Simulator 基准一致。
   - 性能：CSV 输出可归档，后续替换为 msprof/HCCL 真实数据。
   - 可靠性：故障注入、超时、重传、路由切换。

5. Agent 交付材料
   - Prompt 记录。
   - Agent 能力清单。
   - 生成日志。
   - 评委可复现的运行脚本。

## 近期推荐顺序

1. 拓扑感知策略：FullMesh / Hierarchical / Sparse 分类与算法选择。
2. HCCL 适配层文档和 C ABI 占位。
3. AllReduce 的分层算法实现和测试。
4. AllGather / ReduceScatter 的动态分块实现和测试。
5. benchmark CSV 归档脚本和报告模板。
