# Tests

后续补充：

- 算法选择单元测试：`tests/unit/policy_test.cpp`
- 模拟器正确性测试：`tests/unit/simulator_test.cpp`
- 运行时后端测试：`tests/unit/runtime_test.cpp`
- 单机 8 卡 HCCL 集成测试
- 多机 64 卡压测入口

当前环境可用无 CMake 验证：

```bash
./scripts/local_smoke.sh
```
