# CANN/HCCL 环境探测

真实 HCCL 后端接入前，需要先确认目标机器是否具备 CANN/HCCL 开发环境。

## 探测脚本

```bash
./scripts/detect_cann_env.sh
```

脚本会输出：

- `ASCEND_HOME_PATH`
- `ASCEND_TOOLKIT_HOME`
- `LD_LIBRARY_PATH`
- 候选 CANN 根目录
- HCCL / HCOM / ACL 头文件
- HCCL / HCOM / AscendCL 动态库

## 期望找到的关键文件

至少需要确认：

- `hccl.h` 或 `hccl/hccl.h`
- `acl.h` 或 `acl/acl.h`
- `libhccl.so`
- `libascendcl.so`

## 接入判断

如果上述文件存在，下一步可以把路径接入 CMake：

- include path
- library path
- runtime `LD_LIBRARY_PATH`

如果文件不存在，当前工程只能运行 Simulator 后端，不能构建真实 HCCL adapter。
