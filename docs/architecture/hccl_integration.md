# HCCL 集成计划

## 目标

将当前工程从 Simulator 后端推进到真实 CANN/HCCL 插件后端，同时保证没有昇腾环境的机器仍能完成语义验证。

## 当前边界

- C++ 策略接口：`include/cann_liberty/plugin.h`
- C ABI 占位接口：`include/cann_liberty/c_api.h`
- HCCL 适配状态：`include/cann_liberty/hccl_adapter.h`
- 运行时后端：`include/cann_liberty/runtime.h`

## 接入步骤

1. 确认 CANN 环境变量
   - `ASCEND_HOME_PATH`
   - HCCL include 路径
   - HCCL library 路径

2. 确认插件 ABI
   - 通信原语入口名称
   - communicator 类型
   - stream 类型
   - datatype / reduce op 映射

3. 建立真实后端
   - 在 `src/hccl_adapter.cpp` 内用 `CANN_LIBERTY_ENABLE_HCCL` 包住真实头文件。
   - 默认构建继续使用 stub，防止无 CANN 环境无法编译。
   - 真实后端先实现 AllReduce，再扩展 AllGather 和 ReduceScatter。

4. 验证要求
   - 每个真实通信原语必须有 Simulator 对照。
   - 每个性能数据必须记录数据规模、world size、拓扑和算法名。
   - Agent 日志需要记录 Prompt、生成文件、编译命令和测试结果。

## 近期缺口

- 还没有真实 CANN 头文件和库路径。
- 还没有 HCCL 插件 ABI 的官方样例代码。
- 还没有实机拓扑探测数据。
