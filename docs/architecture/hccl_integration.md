# HCCL 集成计划

## 目标

将当前工程从 Simulator 后端推进到真实 CANN/HCCL 插件后端，同时保证没有昇腾环境的机器仍能完成语义验证。

## 当前边界

- C++ 策略接口：`include/cann_liberty/plugin.h`
- C ABI 占位接口：`include/cann_liberty/c_api.h`
- HCCL 适配状态：`include/cann_liberty/hccl_adapter.h`
- 运行时后端：`include/cann_liberty/runtime.h`

当前 HCCL adapter 已提供统一调用边界：

- `HcclCallRequest`
- `HcclCallResult`
- `HcclRuntimeHandles`
- `ExecuteHcclCollective`
- `ExpectedHcclDataType`
- `ExpectedHcclReduceOp`

默认 stub build 会返回 unavailable；真实 CANN/HCCL build 后续在同一函数内完成 lowering。

`HcclRuntimeHandles` 当前用 opaque 指针表达 communicator 和 stream：

- stub build：允许为空，用于保持本地可测试。
- real HCCL build：必须提供非空 communicator 和 stream。

## Lowering 待办

`ExecuteHcclCollective` 后续需要完成：

1. 将 `ExpectedHcclDataType` 的字符串映射替换为真实 HCCL 数据类型 enum。
2. 将 `ExpectedHcclReduceOp` 的字符串映射替换为真实 HCCL reduce op enum。
3. 将 `HcclRuntimeHandles` 的 opaque 指针替换或转换为真实 communicator 和 stream 类型。
4. 按 `CollectiveKind` 调用对应 HCCL 入口。
5. 将 HCCL 返回码转换为 `HcclCallResult`。
6. 为 AllReduce 建立首个实机 smoke test。

## Smoke 入口

当前已提供统一 smoke 入口：

```bash
./scripts/collective_smoke.sh simulator all_reduce 1048576 8
```

输出包含：

- backend
- algorithm
- output_ranks
- rank0_values
- max_abs_error
- check

在真实 HCCL 环境完成 lowering 后，应使用同一入口切换后端：

```bash
./scripts/collective_smoke.sh hccl all_reduce 1048576 8
```

## 接入步骤

1. 确认 CANN 环境变量
   - `ASCEND_HOME_PATH`
   - HCCL include 路径
   - HCCL library 路径
   - 可先运行 `./scripts/detect_cann_env.sh` 生成环境报告。

2. 确认插件 ABI
   - 通信原语入口名称
   - communicator 类型
   - stream 类型
   - datatype / reduce op 映射

3. 建立真实后端
   - 在 `src/hccl_adapter.cpp` 内用 `CANN_LIBERTY_ENABLE_HCCL` 包住真实头文件。
   - 默认构建继续使用 stub，防止无 CANN 环境无法编译。
   - 真实后端先实现 AllReduce，再扩展 AllGather 和 ReduceScatter。

   CMake 入口：

   ```bash
   cmake -S . -B build \
     -DCANN_LIBERTY_ENABLE_HCCL=ON \
     -DCANN_ROOT=/usr/local/Ascend/ascend-toolkit/latest
   ```

   如果自动查找失败，可以显式指定：

   ```bash
   cmake -S . -B build \
     -DCANN_LIBERTY_ENABLE_HCCL=ON \
     -DHCCL_INCLUDE_DIR=/path/to/include \
     -DHCCL_LIBRARY=/path/to/libhccl.so
   ```

4. 验证要求
   - 每个真实通信原语必须有 Simulator 对照。
   - 每个性能数据必须记录数据规模、world size、拓扑和算法名。
   - Agent 日志需要记录 Prompt、生成文件、编译命令和测试结果。

## 近期缺口

- 还没有真实 CANN 头文件和库路径。
- 还没有 HCCL 插件 ABI 的官方样例代码。
- 还没有实机拓扑探测数据。

## 环境探测

详见 `docs/architecture/cann_env_detection.md`。
