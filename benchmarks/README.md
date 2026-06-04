# Benchmarks

保存带宽、延迟、加速比和 Profiling 数据采集脚本。

## 本地策略基准

当前可运行：

```bash
./scripts/benchmark_local.sh
```

输出 CSV 字段：

```text
kind,world_size,bytes_per_rank,total_bytes,algorithm,backend,stage_count,step_count,elapsed_us,sim_mib_per_s
```

该基准只衡量 Simulator 后端的策略调度和 CPU 语义模拟耗时，不代表真实 HCCL/NPU 性能。
