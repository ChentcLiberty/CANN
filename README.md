# CANN Liberty

基于 CANN 的分布式通信算法设计与优化比赛工程。

## 目标

面向昇腾 NPU 集群设计并实现 HCCL 集合通信算法插件，覆盖至少三类核心通信原语：

- AllReduce
- AllGather
- ReduceScatter
- Broadcast
- AlltoAll

工程同时保留 Agent 生成过程、Prompt、运行日志和验证流程，满足比赛对可复现 Agent 工程的要求。

## 目录结构

```text
.
├── agent/                  # Agent 工程、Prompt、日志和能力说明
├── benchmarks/             # 性能测试与压测入口
├── cmake/                  # CMake 辅助模块
├── configs/                # 拓扑、算法策略、模拟器配置
├── deliverables/           # 最终提交材料归档
├── docs/                   # 算法设计、报告、验证说明
├── examples/               # 示例程序
├── include/cann_liberty/   # 对外头文件
├── scripts/                # 构建、测试、打包脚本
├── simulator/              # 无硬件条件下的通信模拟验证
├── src/                    # HCCL 插件与算法实现
├── tests/                  # 单元测试与集成测试
├── third_party/            # 第三方依赖占位
├── tools/                  # 故障注入、Profiling 等工具
└── 赛题描述/               # 原始赛题图片
```

## 当前阶段

- [x] 初始化比赛工程目录
- [x] 建立 Git 仓库准备
- [x] 建立第一版算法选择策略与本地 smoke 测试
- [x] 建立集合通信语义模拟器
- [x] 建立运行时后端抽象与 HCCL 适配占位
- [x] 建立拓扑感知分类与算法选择
- [x] 建立 C ABI 占位接口
- [x] 建立集合通信算法计划层
- [ ] 接入 CANN/HCCL 头文件与编译环境
- [ ] 实现拓扑探测与算法选择框架
- [ ] 实现集合通信算法
- [ ] 建立模拟器验证与性能报告
- [ ] 完成 Agent 工程与交付材料

更详细的推进路线见 `docs/roadmap.md`。

## 本地验证

当前机器如未安装 CMake，可以先运行：

```bash
./scripts/local_smoke.sh
```

本地策略基准：

```bash
./scripts/benchmark_local.sh
```

导出算法通信计划：

```bash
./scripts/dump_plan.sh all_reduce 1048576 8
```
